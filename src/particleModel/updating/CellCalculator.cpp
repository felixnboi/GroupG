#include "CellCalculator.h"
#include "particleModel/storage/CellContainerIterators.h"
#include <iostream>

CellCalculator::CellCalculator(CellContainer &cellContainer, const double delta_t, const std::string& forceType, std::array<boundary_conditions,6> boundaries_cond)
    : cellContainer(cellContainer), cell_size(cellContainer.getCellSize()), 
    delta_t(delta_t), domain_max_dim(cellContainer.getDomain_Max()), boundaries(boundaries_cond) ,
    particles(*cellContainer.getParticles()){
    if (forceType == "LennJones") {
        // preliminary hardcoded
        double sigma{1.0};
        double epsilon{5.0};
        forceLambda = forceLennJonesPotentialFunction(sigma, epsilon);
        force = forceLennJonesPotentialFunction_Ghost(sigma,epsilon);

    } else if (forceType == "simple") {
        forceLambda = forceSimpleGravitational();

    } else {
        throw std::runtime_error("Provided forceType is invalid: " + forceType);
    }
}

void debugLog(int id,CellContainer& cellContainer, std::vector<std::vector<std::vector<std::vector<Particle*>>>>& particles) {
    std::array<dim_t, 3> current_position;

    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> *current_cell = &particles[current_position[0]][current_position[1]][current_position[2]];
        for(Particle* p: *current_cell) {
            if(p->getV()[2] != 0) {
                std::cout<<"In "<<id<<" V is 3 dim!"<<std::endl;
                return;
            }
            if(p->getF()[2] != 0) {
                std::cout<<"In "<<id<<" F is 3 dim!"<<std::endl;
                return;
            }
            if(p->getOldF()[2] != 0) {
                std::cout<<"In "<<id<<" old_F is 3 dim!"<<std::endl;
                return;
            }
        }
        cellContainer.setNextCell(current_position);
    }
    std::cout<<id<<" passed."<<std::endl;
}

void CellCalculator::initializeFX() {
    std::array<dim_t, 3> current_position;
    instructions cell_updates;

    //calculate F
    calculateLinkedCellF();

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> *current_cell = &particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        finishF(current_cell);

        for (auto iter = current_cell->begin(); iter != current_cell->end();) {
            Particle& particle = *(*iter);

            calculateVX(particle, current_position, false);
            particle.shiftF();

            std::array<dim_t, 3> position{
                                  static_cast<dim_t>(particle.getX()[0] / cell_size + 1),
                                  static_cast<dim_t>(particle.getX()[1] / cell_size + 1),
                                  static_cast<dim_t>(particle.getX()[2] / cell_size + 1)};
            
            if (position[0] != current_position[0] ||
                position[1] != current_position[1] ||
                position[2] != current_position[2])
            {
                cell_updates.emplace_back(*iter,position);
                iter = current_cell->erase(iter);

            }else{
                iter++;
            }
        }

        cellContainer.setNextCell(current_position);
    }


    std::cout << "After loop before update" << cellContainer.to_string() << std::endl;
    //update particle distribution in the cells
    updateCells(cell_updates);
}

void CellCalculator::initializeFX_simple() {
    static instructions cell_updates;

    //calculate F
    calculateLinkedCellF_simple();

    //write new coordinates in current_position array

    for(auto iter = cellContainer.begin(); iter != cellContainer.end(); ++iter){
        std::vector<Particle*> *current_cell = &particles[iter.x][iter.y][iter.z];

        //finish F calculation
        finishF(current_cell);

        for (auto iter_cell = current_cell->begin(); iter_cell != current_cell->end();) {
            Particle* particle_ptr = *iter_cell;
            Particle& particle = *particle_ptr;
            std::array<double, 3> x = particle.getX();
            std::array<double, 3> f = particle.getF();
            std::array<double, 3> v = particle.getV();
            double m = particle.getM();

            for(int i = 0; i < 3; i++){
              particle.setX(i,x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
            }


            particle.shiftF();

            x = particle.getX();
            std::array<dim_t,3> new_pos;
            cellContainer.allocateCell(x,new_pos);
            if(new_pos[0] != x[0] || new_pos[1] != x[1] || new_pos[2] != x[2]){
                iter_cell = current_cell->erase(iter_cell);
                std::tuple<Particle*,std::array<dim_t,3>> to_insert_again(particle_ptr,new_pos);
                cell_updates.push_back(to_insert_again);
            }else{
                ++iter_cell;
            }
        }
    }


    //std::cout << "After loop before update" << cellContainer.to_string() << std::endl;
    //update particle distribution in the cells
    updateCells_simple(cell_updates);
}



void CellCalculator::calculateLinkedCellF() {
    static std::array<dim_t, 3> current;
    static std::array<dim_t, 3> pattern;
    std::array<double, 3> F_ij{};
    //write new path inside current/start and pattern
    cellContainer.setNextPath(current, pattern);

    while(current[0] != dim_t_res) {
        std::vector<Particle*>* cell_1 = &particles[current[0]][current[1]][current[2]];
        current[0] += pattern[0];
        current[1] += pattern[1];
        current[2] += pattern[2];

        while(0 < current[0] && 0 < current[1] && 0 < current[2] && current[0] <= domain_max_dim[0]
              && current[1] <= domain_max_dim[1] && current[2] <= domain_max_dim[2]) {

            std::vector<Particle*>* cell_2 = &particles[current[0]][current[1]][current[2]];

            for(auto p_i = cell_1->begin(); p_i != cell_1->end(); p_i++) {
                for(auto p_j = cell_2->begin(); p_j != cell_2->end(); p_j++) {

                    F_ij = forceLambda(*(*p_i), *(*p_j));

                    for (int i = 0; i < 3; i++) {
                        (*p_i)->addF(i, F_ij[i]);
                        (*p_j)->addF(i, -F_ij[i]);
                    }
                }
            }

            cell_1 = &particles[current[0]][current[1]][current[2]];
            current[0] += pattern[0];
            current[1] += pattern[1];
            current[2] += pattern[2];
        }

        cellContainer.setNextPath(current, pattern);
    }
}

void CellCalculator::calculateForces_between_two_Cells(std::vector<Particle*> *cell1,std::vector<Particle*> *cell2){
  double cut_of_radius = cellContainer.getCutOfRadius();
  for(Particle* particle1 : *cell1){
    for(Particle* particle2 : *cell2){
      double distance = ArrayUtils::L2Norm((*particle1).getX() - (*particle2).getX());
      if(distance < cut_of_radius){
          std::array<double, 3> F_ij = forceLambda(*particle1,*particle2);
          for (int i = 0; i < 3; i++) {
                particle1->addF(i, F_ij[i]);
                particle2->addF(i, -F_ij[i]);
          }
      }
    }
  }

}

void CellCalculator::calculateLinkedCellF_simple(){
  dim_t comparing_depth = cellContainer.getComparingdepth();

  auto iter = cellContainer.begin();

  while(iter != cellContainer.end()) {
    std::vector<Particle*> *current_cell = &particles[iter.x][iter.y][iter.z];

    dim_t x,y,z;
    x = iter.x;
    y = iter.y+1;
    z = iter.z;

    while(x <= iter.x + comparing_depth){
      if(cellContainer.is_valid_domain_cell(x,y,z)){
      std::vector<Particle*> *other_cell = &particles[x][y][z];
      calculateForces_between_two_Cells(current_cell,other_cell);
      }

      y++;
      if(iter.y+comparing_depth  < y){
        y = iter.y-comparing_depth;
        x++;
      }
    }

    ++iter;
  }
}

void CellCalculator::calculateWithinFVX_simple(){
  instructions cell_updates;

  for(auto iter = cellContainer.begin(); iter != cellContainer.end() ; ++iter){
    std::vector<Particle*> *current_cell = &particles[iter.x][iter.y][iter.z];
    finishF(current_cell);
    //calculate V
    for(auto iter_cell = current_cell->begin(); iter_cell != current_cell->end();){
        Particle* particle_ptr = *iter_cell;
        Particle& particle = *particle_ptr;
        std::array<double, 3> x = particle.getX();
        std::array<double, 3> f = particle.getF();
        std::array<double, 3> f_old = particle.getOldF();
        std::array<double, 3> v = particle.getV();
        const double m = particle.getM();

        for (int i = 0; i < 3; i++) {
            particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }

        v = particle.getV();

        for(int i = 0; i < 3; i++){
            particle.setX(i,x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
        }

        particle.shiftF();
        x = particle.getX();
        std::array<dim_t,3> new_pos;
        cellContainer.allocateCell(x,new_pos);
        if(new_pos[0] != x[0] || new_pos[1] != x[1] || new_pos[2] != x[2]){
            iter_cell = current_cell->erase(iter_cell);
            std::tuple<Particle*,std::array<dim_t,3>> to_insert_again(particle_ptr,new_pos);
            cell_updates.push_back(to_insert_again);
        }else{
            ++iter_cell;
        }

    }

  }
  updateCells_simple(cell_updates);
}

void CellCalculator::calculateWithinFVX() {
    std::array<dim_t, 3> current_position;
    instructions cell_updates;
    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);
    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> *current_cell = &particles[current_position[0]][current_position[1]][current_position[2]];
        //finish F calculation
        finishF(current_cell);

        for (auto iter = current_cell->begin(); iter != current_cell->end();) {
            Particle& particle = *(*iter);
            calculateVX(particle, current_position, true);
            particle.shiftF();
            std::array<dim_t, 3> position{
                                  static_cast<dim_t>((particle.getX())[0] / cell_size + 1),
                                  static_cast<dim_t>((particle.getX())[1] / cell_size + 1),
                                  static_cast<dim_t>((particle.getX())[2] / cell_size + 1)};

            if (position[0] != current_position[0] ||
                position[1] != current_position[1] ||
                position[2] != current_position[2])
            {
                cell_updates.emplace_back(*iter,position);
                iter = current_cell->erase(iter);

            } else{
                iter++;
            }
        }
        cellContainer.setNextCell(current_position);
    }
    updateCells(cell_updates);
}

void CellCalculator::updateCells(instructions& cell_updates) {
    //todo check for outflow here
    while(!cell_updates.empty()) {
        std::tuple<Particle* , std::array<dim_t,3>> move_instruction  = cell_updates.back();
        Particle* particle_ptr = std::get<0>(move_instruction);
        std::array<dim_t, 3> new_cell_position = std::get<1>(move_instruction);
        std::vector<Particle*> *new_cell = &particles[new_cell_position[0]][new_cell_position[1]][new_cell_position[2]];
        new_cell->push_back(particle_ptr);
        cell_updates.pop_back();
    }
    
}

void CellCalculator::updateCells_simple(instructions& cell_updates) {

    static size_t amt_removed = 0;
    for(auto ins : cell_updates){
      Particle* particle_ptr = std::get<0>(ins);
      std::array<dim_t, 3> new_cell_position = std::get<1>(ins);

      if( 0 < new_cell_position[0] &&  new_cell_position[0] < static_cast<int>(particles.size()) &&
            0 < new_cell_position[1] &&  new_cell_position[1] < static_cast<int>(particles[0].size()) &&
            0 < new_cell_position[2] && new_cell_position[2] < static_cast<int>(particles[0][0].size())) {
          std::vector<Particle*> *new_cell = &particles[new_cell_position[0]][new_cell_position[1]][new_cell_position[2]];
          new_cell->push_back(particle_ptr);
      }
      else{
          cellContainer.getHaloParticles().push_back(particle_ptr);
          amt_removed++;
          std::cout << "Oh it's not a valid domain cell, removed " << amt_removed << "\n";
          std::cout << "could not add at: " << new_cell_position[0] << " , " << new_cell_position[1] << " , " << new_cell_position[2] << "\n";
          std::cout << "The particle: " << (*particle_ptr).toString() << "\ntrace:\n";
          // while(!(*particle_ptr).trace.empty()){
          //   std::array<double,3> arr = (*particle_ptr).trace.front();
          //   std::cout << arr[0] << " , " << arr[1] << " , "  << arr[2] << "\n";
          //   (*particle_ptr).trace.pop();
          // }
          // std::cout << "Trace of force " << std::endl;
          // while(!(*particle_ptr).trace_force.empty()){
          //   std::array<double,3> arr = (*particle_ptr).trace_force.front();
          //   std::cout << arr[0] << " , " << arr[1] << " , "  << arr[2] << "\n";
          //   (*particle_ptr).trace_force.pop();
          // }
      }
    }
}

void CellCalculator::calculateVX(Particle &particle, std::array<dim_t, 3> &current_position,
                                bool calculateV) {
    const std::array<double, 3> &f = particle.getF();
    const std::array<double, 3> &x = particle.getX();
    const std::array<double, 3> &v = particle.getV();
    const double &m = particle.getM();

    if(calculateV) {
        const std::array<double, 3> &f_old = particle.getOldF();

        //calculate V
        for (int i = 0; i < 3; i++) {
            particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }
    }

    //calculate X
    double x_0 = x[0] + delta_t * v[0] + delta_t * delta_t * f[0] / 2.0 / m;
    double x_1 = x[1] + delta_t * v[1] + delta_t * delta_t * f[1] / 2.0 / m;
    double x_2 = x[2] + delta_t * v[2] + delta_t * delta_t * f[2] / 2.0 / m;

    particle.setX(0, x_0);
    particle.setX(1, x_1);
    particle.setX(2, x_2);
}


void CellCalculator::finishF(std::vector<Particle*> *current_cell) {
    Particle* p_i;
    Particle* p_j;
    std::array<double, 3> F_ij{};

    for (auto it1 = current_cell->begin(); it1 != current_cell->end(); ++it1) {
        for(auto it2 = std::next(it1); it2 != current_cell->end(); ++it2) {
            p_i = *it1;
            p_j = *it2;

            F_ij = forceLambda(*p_i, *p_j);

            for (int i = 0; i < 3; i++) {
                p_i->addF(i, F_ij[i]);
                p_j->addF(i, -F_ij[i]);
            }
        }
    }
}


//Top and Bottom
void CellCalculator::calculateBoundariesTopOrBottom(dim_t lower_z,dim_t upper_z, dim_t z_border){
    dim_t x, y;
    x = y =  1;

  // bottom boundary
  while(lower_z <= upper_z){
  while (y < domain_max_dim[1]) {
    std::vector<Particle*>& cell = particles[x][y][lower_z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
      double distance =z_dim - z_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_z = z_dim - 2*distance;

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,y_dim,ghost_particle_z});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    x++;
    if (x >= domain_max_dim[0]) {
      x = 1;
      y++;
    }
  }
  lower_z++;
  }

};

//Front and Back
void CellCalculator::calculateBoundariesFrontOrBack(dim_t lower_x,dim_t upper_x ,dim_t x_border, dim_t z_until){
    dim_t y, z;
    z = y =  1;

  // bottom boundary
  while(lower_x <= upper_x){
  while (z <= z_until) {
    std::vector<Particle*>& cell = particles[lower_x][y][z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance = x_dim - x_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_x = x_dim - 2 * distance;

        std::array<double,3> F_particle_ghost = force(particle,{ghost_particle_x,y_dim,z_dim});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    y++;
    if (y >= domain_max_dim[1]) {
      y = 1;
      z++;
    }
  }
  lower_x++;
  }


}; //Front and Back

//Left and Right
void CellCalculator::calculateBoundariesLeftOrRight(dim_t lower_y,dim_t upper_y ,dim_t y_border, dim_t z_until){
    dim_t x, z;
    z = x =  1;

  // bottom boundary
  while(lower_y <= upper_y){
  while (z <= z_until) {
    std::vector<Particle*>& cell = particles[x][lower_y][z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance = y_dim - y_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_y = y_dim - 2 * distance;

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,ghost_particle_y,z_dim});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    x++;
    if (x >= domain_max_dim[0]) {
      x = 1;
      z++;
    }
  }
  lower_y++;
  }
}; //Left and Right


/**
 * 
 * @brief calculates reflecting boundary conditions by applying Ghost Particles
 * 
 * This method assumes, that at the moment it is called all particles are within the 
 * domain boundaries. It a particle has coordinates outside the domain it is undefined behaviour.
 * 
 * 
 * 
 * 
*/
void CellCalculator::applyGhostParticles() {
  auto domain_border = cellContainer.getDomainBounds();
  dim_t  z_max = cellContainer.hasThreeDimensions() ? domain_max_dim[2] : 1;
  dim_t comparing_depth = cellContainer.getComparingdepth();
  if(cellContainer.hasThreeDimensions()){

  //boundaries[0] corresponds to boundary_conditions in positiveZ direction
  if(boundaries[0] == boundary_conditions::reflective)
    calculateBoundariesTopOrBottom(domain_max_dim[2],comparing_depth,domain_border[2]); //Top

  //boundaries[1] corresponds to boundary_conditions in negativeZ direction
  if(boundaries[1] == boundary_conditions::reflective)
    calculateBoundariesTopOrBottom(1,comparing_depth,0); //Bottom


  }

  //boundaries[2] corresponds to boundary_conditions in positiveX direction
  if(boundaries[2] == boundary_conditions::reflective) 
    calculateBoundariesFrontOrBack(domain_max_dim[0],comparing_depth,domain_border[0],z_max); //Back

  //boundaries[3] corresponds to boundary_conditions in negativeX direction
  if(boundaries[3] == boundary_conditions::reflective)
    calculateBoundariesFrontOrBack(1,comparing_depth,0,z_max); //Front

  //boundaries[4] corresponds to boundary_conditions in positiveY direction
  if(boundaries[4] == boundary_conditions::reflective)
    calculateBoundariesLeftOrRight(domain_max_dim[1],comparing_depth,domain_border[1],z_max); //Right

  //boundaries[5] corresponds to boundary_conditions in negativeY direction
  if(boundaries[5] == boundary_conditions::reflective) 
    calculateBoundariesLeftOrRight(1,comparing_depth,0,z_max); //Left
  
}