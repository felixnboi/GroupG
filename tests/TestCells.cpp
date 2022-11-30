#include <gtest/gtest.h>

#include "data/ParticleContainer.h"
#include "data/Particle.h"
#include "io/output/Logging.h"

#include <array>
#include <vector>
#include <algorithm>

/**
 * @brief Helper function that initializes a tiny particleContainer for testing
 * 
 */
ParticleContainer initializeTestPContainer(){
    std::vector<Particle> particles{};
    double r_cutoff = 1.2;
    for(double x=0.; x < 3; x++){
        for(double y=0.; y<3;y++){
            for(double z=0.; z<3; z++){
                Eigen::Vector3d pos{r_cutoff*x + 0.1, r_cutoff*y + 0.1,r_cutoff*z + 0.1};
                Eigen::Vector3d vel{0.,0.,0.};
                particles.emplace_back(pos, vel, 1.0, 0);
            }
        }
    }
    return ParticleContainer(particles, {3.1, 3.1, 3.1}, r_cutoff);
}

/**
 * Test if the ParticleContainer initializes the Cell-Datastructure as intended 
 * (with the proper amount of cells, the right dimensions, the particles in the right cells, etc) 
 * 
 */
TEST(ParticleContainer, Cell_Initialization){
    ParticleContainer particleContainer = initializeTestPContainer();

    ASSERT_EQ(particleContainer.size(), 27) << "ParticleContainer did not get initialized with the right amount of items!";
    std::array<unsigned int, 3> expected_dimensions{3,3,3};
    ASSERT_EQ(particleContainer.getGridDimensions(), expected_dimensions) << "ParticleContainer did not initialize the grid with the right dimensions";

    //check if every cell actually has one element
    particleContainer.forAllCells([&](std::vector<double> &, std::vector<double> &, std::vector<double> &, std::vector<double> &,
        std::vector<double> &, std::vector<int> &, unsigned long , std::vector<size_t> &cellItems){
        ASSERT_EQ(cellItems.size(), 1)<<"ParticleContainer did not assign the right amount of Particles into each cell";    
    });
}

/**
 * Test if forAllNeighbouringCells actually lets you interact with all Neighbouring cells
 * 
 */
TEST(ParticleContainer, forAllDistinctCellNeighbours){
    //spdlog::set_level(static_cast<spdlog::level::level_enum>(6));

    //initialize 3x3 grid with one particle in each cell
    ParticleContainer particleContainer = initializeTestPContainer();

    //initialize variables to store interaction state so far
    std::array<bool, 27> cellFlags;
    std::fill(cellFlags.begin(), cellFlags.end(), false);
    unsigned int cellInQuestion = 1+3+9;

    //Define alternative functions that sef flags accordingly to interactions ---------------------------------------------------
    auto neighbourChecker = [&cellFlags,&cellInQuestion](std::vector<double> &, std::vector<double> &, std::vector<double> &,
                                             std::vector<double> &, std::vector<double> &, std::vector<int> &,
                                             unsigned long, std::vector<unsigned long>& cell0Items, std::vector<unsigned long>& cell1Items){
        if(cell1Items.size()!=1||cell0Items.size()!=1){
            return;
        }
        if(cell0Items[0] == cellInQuestion){
            cellFlags[cell1Items[0]] = true;
        }
        else if(cell1Items[0] == cellInQuestion){
            cellFlags[cell0Items[0]] = true;
        }                                            
    };
    auto cellChecker = [&cellFlags, &cellInQuestion](std::vector<double> &, std::vector<double> &, std::vector<double> &,
                                             std::vector<double> &, std::vector<double> &, std::vector<int> &,
                                             unsigned long, std::vector<unsigned long>& cellItems){
        if(cellItems.size()!=1){
            return;
        }
        if(cellItems[0] == cellInQuestion){
            cellFlags[cellInQuestion] = true;
        }
    };
    //end of helper function definitions --------------------------------------------

    //check that middle interacted with literally everyone else
    particleContainer.forAllDistinctCellNeighbours(neighbourChecker);
    for(unsigned int x=0; x<3; x++){
        for(unsigned int y=0; y<3; y++){
            for(unsigned int z=0; z<3; z++){
                //don't interact with yourself
                if(particleContainer.cellIndexFromCellCoordinates({x,y,z}) == cellInQuestion){
                    ASSERT_EQ(false, cellFlags[particleContainer.cellIndexFromCellCoordinates({x,y,z})])<<"Particle in cell {1,1,1} interacted with itself when supposedly interacting with Neighbours only";
                }
                //but with everyone else
                else{
                    //std::cout<<"Checking iteraction with ("<< x << ", " << y << ", " << z << ")"<< '\n';
                    //std::cout<< "Looking at index " << particleContainer.cellIndexFromCellCoordinates({x,y,z}) << " which has the value " << neighbourChecker.cellFlags[particleContainer.cellIndexFromCellCoordinates({x,y,z})] << '\n';
                    ASSERT_EQ(true, cellFlags[particleContainer.cellIndexFromCellCoordinates({x,y,z})])<<"Particle in cell {1,1,1} did not interact with all of its Neighbours";
                }
            }
        }
    }

    
    //check that you can interact with yourself:
    particleContainer.forAllCells(cellChecker);
    ASSERT_EQ(true, cellFlags[particleContainer.cellIndexFromCellCoordinates({1,1,1})])<<"Particle in cell {1,1,1} did not interact with itself when it was supposed to do so";

    //check that bottom front left cell interacts with its neighbours and itself properly without interacting with others:
    std::fill(cellFlags.begin(), cellFlags.end(), false);
    cellInQuestion = particleContainer.cellIndexFromCellCoordinates({0,0,0});

    particleContainer.forAllCells(cellChecker);
    particleContainer.forAllDistinctCellNeighbours(neighbourChecker);
    
    for(unsigned int x=0; x<3; x++){
        for(unsigned int y=0; y<3; y++){
            for(unsigned int z=0; z<3; z++){
                //don't interact if you are no neighbour
                if(x >= 2 || y >= 2 || z >= 2){
                    ASSERT_EQ(false, cellFlags[particleContainer.cellIndexFromCellCoordinates({x,y,z})])<<"Particle in cell {0,0,0} interacted with a cell that was not its Neighbour";
                }
                //but do interact if you actually are a neighbour
                else{
                    ASSERT_EQ(true, cellFlags[particleContainer.cellIndexFromCellCoordinates({x,y,z})])<<"(0 0 0) did not interact with ( " << x << " " << y << " " << z << ")\n";
                }
            }
        }
    }
}
