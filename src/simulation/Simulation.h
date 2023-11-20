#pragma once

#include <memory>
#include <vector>

#include "containers/DirectSumContainer.h"
#include "integration/IntegrationFunctor.h"
#include "io/output/FileOutputHandler.h"

/**
 * @brief Class to run a simulation
 *
 * This class collects all the components needed to run a simulation, and provides a method to run it.
 */
class Simulation {
    DirectSumContainer& particles;
    const double delta_t;
    const double simulation_end_time;

    const FileOutputHandler file_output_handler;
    const int fps;
    const int video_length;

    const std::vector<std::unique_ptr<ForceSource>>& forces;
    std::unique_ptr<IntegrationFunctor> integration_functor;

   public:
    enum class IntegrationMethod { VERLET };

    /**
     * @brief Construct a new Simulation object and initialize all the necessary components
     * @param particles Reference to the `DirectSumContainer` on whose content the simulation is performed
     * @param forces Vector of forces which are applied in the simulation
     * @param file_output_handler Reference to the output handler used for writing the output files
     * @param delta_t Time step per iteration
     * @param simulation_end_time End time of the simulation
     * @param integration_method Integration method to use (default: VERLET)
     * @param fps Frames per second at which to save the simulation
     * @param video_length Length of the simulation video in seconds
     */
    Simulation(DirectSumContainer& particles, const std::vector<std::unique_ptr<ForceSource>>& forces,
               FileOutputHandler& file_output_handler, double delta_t, double simulation_end_time, int fps = 24, int video_length = 30,
               IntegrationMethod integration_method = IntegrationMethod::VERLET);

    /**
     * @brief Runs the simulation, using the parameters given at construction
     */
    void runSimulation() const;
};