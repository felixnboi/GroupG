#pragma once

#include "particleModel/storage/ParticleContainer.h"
#include "particleModel/updating/Calculator.h"
#include "utils/ForceCalculations.h"
#include <functional>

/**
 * @brief Offers important functions for particle interactions.
 *
 * This class simulates the interaction between particles according to the specified calculations.
 * It offers the functionality to calculate the forces, velocities and position of particles. 
 */
class Model : public Calculator {
public:
    Model(ParticleContainer& particleContainer,  const double delta_t, ForceCalculation force);


    void initalize() override;
    void iteration() override;

    /**
     *
     * @brief calculates the force of every Particle at the current time step
     *
     * for every particle all the forces acting on that particle are summed up, while
     * iterating over all unique pairs of particles {p_i,p_j}, where p_i != p_j. Meaning
     * for the pair {p_i,p_j}, the force f_i_j is calulated and added / substracted
     * to F_i / F_j (on which added/substracted depends on order,in which iterated over the particles)
     * After that the next f_i_j for the next unique pair is calculated
     * sigma and epsilon are only needed to Lennard-Jones-Potential
     *
     * @param forcecalc The function with which the forces between every two particles are calculated
     * @return None
     *
     */
    void calculateF();

    /**
     *
     * @brief calculates the position of every particle for the next timestep according to given formula
     *
     *
     * @param None
     * @return None
     */
    void calculateX();

    /**
     * @brief calculates the velocity of every particle for the next timestep according to given formula
     *
     * @param None
     * @return None
     */
    void calculateV();

    /**
     *
     * @brief for every particle change old force attribute to the current force
     *
     * called after every iteration/ when new X,V and F was calculated
     * sets  the Particle.old_f (old force) for the next time step to be the Particle.f (current force) of this
     * time step
     *
     * @param None
     * @return None
     *
     */
    void shiftForces();


    private:
        ParticleContainer simulationContainer;

};
