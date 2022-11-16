/*
 * LennardJonesSimulation.cpp 
 * 
 * Created: 16.11.2022 
 * Author:  marquardt 
*/

#include "LennardJonesSimulation.h"
#include "../model/ParticleContainer.h"
#include "../utils/ArrayUtils.h"

void LennardJonesSimulation::calculateF() {
    ParticleContainer *particleContainer = this->getParticleContainer();

    //values hardcoded for now
    double epsilon = 5;
    double sigma = 1;

    // first we iterate over each particle once to initialize new force vector to zero
    std::function<void (Particle &)> forceInitializationIteration = [] (Particle &p1) {
        p1.setOldF(p1.getF());
        p1.setF({0.0 , 0.0, 0.0});
    };

    particleContainer->iterateParticles(forceInitializationIteration); 

    // in the second step we calculate the forces between pairs of particles according to the Lennard-Jones formula
    std::function<void (Particle&, Particle&)> forceCalculationIteration = [epsilon, sigma] (Particle &p1, Particle &p2) {
        double pow1 = sigma / ArrayUtils::L2Norm(p1.getX() - p2.getX());
        double pow2 = pow1 * pow1;
        double pow4 = pow2 * pow2;
        double pow6 = pow4 * pow2;
        double pow12 = pow6 * pow6;

        std::array<double,3> f_ij = -1 * (24 * epsilon / pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 2)) * (pow6 - 2*pow12) * (p1.getX() - p2.getX());
        std::array<double,3> f_ji = -1 * f_ij;
        
        p1.addF(f_ij);
        p2.addF(f_ji);
    };
    
    particleContainer->iterateParticlePairs(forceCalculationIteration);

}