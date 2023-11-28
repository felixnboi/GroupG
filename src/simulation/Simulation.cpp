#include "Simulation.h"

#include <chrono>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "io/logger/Logger.h"

Simulation::Simulation(std::unique_ptr<ParticleContainer>& particles, const std::vector<std::unique_ptr<ForceSource>>& forces,
                       const FileOutputHandler& file_output_handler, double delta_t, double simulation_end_time, int fps, int video_length,
                       IntegrationMethod integration_method)
    : particles(particles),
      delta_t(delta_t),
      simulation_end_time(simulation_end_time),
      file_output_handler(file_output_handler),
      fps(fps),
      video_length(video_length),
      forces(forces) {
    switch (integration_method) {
        case IntegrationMethod::VERLET:
            integration_functor = std::make_unique<VerletFunctor>();
            break;
        default:
            Logger::logger->error("Integration method not implemented.");
            exit(1);
    }
}

Simulation::Simulation(std::unique_ptr<ParticleContainer>& particles, const std::vector<std::unique_ptr<ForceSource>>& forces,
                       const SimulationParams& simulation_params, IntegrationMethod integration_method)
    : Simulation(particles, forces, FileOutputHandler(simulation_params.output_format, simulation_params.output_dir_path),
                 simulation_params.delta_t, simulation_params.end_time, simulation_params.fps, simulation_params.video_length,
                 integration_method) {}

SimulationOverview Simulation::runSimulation() const {
    int iteration = 0;
    double simulation_time = 0;

    const size_t expected_iterations = simulation_end_time / delta_t;
    const size_t fill_width = log10(expected_iterations) + 1;

    bool no_output = fps == 0 || video_length == 0;

    const size_t save_every_nth_iteration =
        no_output ? std::numeric_limits<size_t>::max() : std::max(expected_iterations / (fps * video_length), 1ul);

    Logger::logger->info("Simulation started...");

    // Calculate initial forces
    particles->applyPairwiseForces(forces);

    // keep track of time for progress high precision
    auto start_time = std::chrono::high_resolution_clock::now();
    auto t_now = start_time;
    auto t_prev = start_time;

    while (simulation_time < simulation_end_time) {
        if (!no_output && iteration % save_every_nth_iteration == 0) {
            // calculate time since last write
            t_now = std::chrono::high_resolution_clock::now();
            const double seconds_since_last_write = std::chrono::duration<double>(t_now - t_prev).count();
            t_prev = t_now;

            // calculate estimated remaining time
            const size_t estimated_remaining_seconds =
                (expected_iterations - iteration) * seconds_since_last_write / save_every_nth_iteration;

            const size_t percentage = 100 * iteration / expected_iterations;

            std::string estimated_remaining_time;
            if (estimated_remaining_seconds >= 0) {
                int hours = estimated_remaining_seconds / 3600;
                int minutes = (estimated_remaining_seconds % 3600) / 60;
                int seconds = estimated_remaining_seconds % 60;

                estimated_remaining_time = fmt::format("(ETA: {:02d}:{:02d}:{:02d})", hours, minutes, seconds);
            }

            Logger::logger->info("Iteration {:>{}}/{} finished   {:>3}% {}", iteration, fill_width, expected_iterations, percentage,
                                 estimated_remaining_time);

            // write output
            file_output_handler.writeFile(iteration, particles);
        }

        integration_functor->step(particles, forces, delta_t);

        iteration++;
        simulation_time += delta_t;
    }

    Logger::logger->info("Simulation finished.");

    auto total_simulation_time_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
    return SimulationOverview{total_simulation_time_millis / 1000.0, total_simulation_time_millis / static_cast<double>(iteration),
                              static_cast<size_t>(iteration), expected_iterations / save_every_nth_iteration};
}
