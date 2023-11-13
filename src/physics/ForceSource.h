#pragma once

#include "types/Particle.h"

/**
 * @brief Interface for force source classes
 *
 * Definition of an interface for force source classes.
 * This is used to ensure inheriting classes implement the method calculateForce(ParticleContainer& particles)
 * according to our definition.
 *
 * A ForceSource specifies the force a particle q exerts on another particle p.
 */
class ForceSource {
   public:
    /**
     * @brief Virtual destructor
     *
     * Virtual destructor to ensure correct deletion of inheriting classes.
     */
    virtual ~ForceSource() = default;

    /**
     * @brief Calculates the force a particle q exerts on another particle p
     * @param p Particle whose force is to be updated
     * @param q Particle which exerts the force on p
     * @return Force exerted by q on p
     *
     * Calculates the force a particle q exerts on another particle p.
     */
    virtual std::array<double, 3UL> calculateForce(Particle& p, Particle& q) = 0;

    /**
     * @brief Returns the name of the force source
     * @return Name of the force source
     *
     * Returns the name of the force source.
     */
    virtual std::string getName() = 0;
};