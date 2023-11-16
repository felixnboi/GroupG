//
// Created by Alp Kaan Aksu on 29.10.23.
//

#pragma once

#include <vector>
#include <functional>

#include "models/Particle.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * ParticleContainer class
 * A data structure that stores all particles in simulations
 * and provides a convenient way to iterate over (pairs of) them
 */
class ParticleContainer {
private:
    /**
     * The vector that contains all the particles in the container
     */
    std::vector<Particle> particles;
public:
    ParticleContainer();

    /**
     * @brief Apply a function to all particles (one by one)
     *
     * @param function
     */
    void applyToAll(const std::function<void(Particle &)>& function);

    /**
     * @brief Apply a function to all pairs of particles
     *
     * @note The function is applied to each pair twice, once for each particle. You might want to use applyToAllPairsOnce() instead.
     *
     * @param function
     */
    void applyToAllPairs(const std::function<void(Particle &, Particle &)> &function);

    /**
     * @brief Add a new particle to the container
     *
     * @param particle
     */
    void add(const Particle &particle);

    /**
     * @brief Add particles specified in a json object to the container
     *
     * @param objects typically a json array of objects
     */
    void add(const json &objects);

    /**
     * @brief Return the number of particles in the container
     *
     * @return Number of particles
     */
    int size();

    /**
     * @brief Apply a function to all unique pairs of particles
     *
     * @param function
     */
    void applyToAllPairsOnce(const std::function<void(Particle &, Particle &)> &function);

    /**
     * @brief Remove a particle from the container
     *
     * @param particle
     */
    void remove(Particle &particle);

    /**
     * @brief Get the internal particle vector
     *
     * @note This is not a copy, but the actual vector. Use with caution. Try to avoid manipulating the vector directly. If possible, use 'apply' family of functions instead.
     *
     * @return Internal particle vector
     */
    const std::vector<Particle> &getParticles() const;
};