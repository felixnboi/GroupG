//
// Created by alex on 30.10.2022.
//

#pragma once

#include "defaults.h"
#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "io/output/VTKWriter.h"
#include "io/output/Logging.h"
#include "io/IOWrapper.h"
#include "io/input/sim_input/BodyReader.h"
#include "sim/physics/force/FLennardJonesOMP.h"
#include "sim/physics/force/types.h"
#include "sim/physics/position/XStoermerVelvetOMP.h"
#include "sim/physics/velocity/VStoermerVelvetOMP.h"
#include "sim/physics/bounds/types.h"
#include "sim/physics/bounds/BoundsFunctorBase.h"
#include "sim/physics/bounds/BoundsHandler.h"
#include "sim/physics/position/types.h"
#include "sim/physics/velocity/types.h"

#include <memory>
#include <chrono>

namespace sim {
    /**
     * Wrapper for the actually used implementations during the simulation for the different calculation methods.
     * */
    class Simulation {
    private:
        ParticleContainer &particleContainer;
        double start_time;
        double end_time;
        double delta_t;
        double epsilon;
        double sigma;
        const std::string &outputFolder;
        const std::string &outputBaseName;
        const bool linkedCell;
        physics::force::ForceFunctorBase* p_calcF;
        physics::PhysicsFunctorBase* p_calcX;
        physics::PhysicsFunctorBase* p_calcV;

    public:
        physics::force::ForceFunctorBase& calcF;
        physics::PhysicsFunctorBase& calcX;
        physics::PhysicsFunctorBase& calcV;
        sim::physics::bounds::BoundsHandler handleBounds;

        /**
         * Standard constructor with all params.
         * */
        explicit Simulation(ParticleContainer &pc, double st = default_start_time, double et = default_end_time,
                   double dt = default_delta_t, double eps = default_epsilon, double sig = default_sigma,
                   const std::string &of = std::string{default_output_folder},
                   const std::string &on = std::string{default_output_base_name},
                   sim::physics::bounds::type leftBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::bounds::type rightBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::bounds::type topBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::bounds::type botBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::bounds::type frontBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::bounds::type rearBound = sim::physics::bounds::stot(default_boundary_cond_str),
                   sim::physics::force::type forceType = sim::physics::force::stot(default_force_type),
                   sim::physics::position::type posType = sim::physics::position::stot(default_pos_type),
                   sim::physics::velocity::type velType = sim::physics::velocity::stot(default_vel_type),
                   bool lc = default_linked_cell) :
                particleContainer(pc),
                start_time(st), end_time(et),
                delta_t(dt), epsilon(eps),
                sigma(sig), outputFolder(of),
                outputBaseName(on), linkedCell(lc),
                p_calcF(sim::physics::force::generateForce(forceType, st, et ,dt, eps, sig, pc)),
                p_calcX(sim::physics::position::generatePosition(posType, st, et ,dt, eps, sig, pc)),
                p_calcV(sim::physics::velocity::generateVelocity(velType, st, et ,dt, eps, sig, pc)),
                calcF(*p_calcF),
                calcX(*p_calcX),
                calcV(*p_calcV),
                handleBounds(leftBound, rightBound, topBound, botBound, frontBound, rearBound,
                             calcF, st, et, dt, eps, sig, pc){
            if(p_calcF == nullptr || p_calcX == nullptr || p_calcV == nullptr) {
                io::output::loggers::general->error("Failed to initialize simulation. Malloc failed.");
                exit(-1);
            }
        }

        /**
         * Constructor with no boundary information. Will init simulation, s.t. no linked cell will be used.
         * */
        explicit Simulation(ParticleContainer &pc, double st, double et, double dt, double eps, double sig,
                   const std::string &of, const std::string &on,
                   sim::physics::force::type forceType = sim::physics::force::stot(default_force_type),
                   sim::physics::position::type posType = sim::physics::position::stot(default_pos_type),
                   sim::physics::velocity::type velType = sim::physics::velocity::stot(default_vel_type)) :
                   Simulation(pc, st, et, dt, eps, sig, of, on,
                              sim::physics::bounds::type::outflow,
                              sim::physics::bounds::type::outflow,
                              sim::physics::bounds::type::outflow,
                              sim::physics::bounds::type::outflow,
                              sim::physics::bounds::type::outflow,
                              sim::physics::bounds::type::outflow,
                              forceType, posType, velType, false) {}

        /**
         * Need to clean up due to polymorphism.
         * */
        ~Simulation() {
            delete p_calcF;
            delete p_calcX;
            delete p_calcV;
        }

        /**
         * Runs the simulation loop
         * @param writeParticle Function to write all particles all 10 iterations
         * */
        void run(const std::function<void(ParticleContainer &pc, const std::string &outputFolder,
                                          const std::string &outputBaseName,
                                          int iteration)> &writeParticle) {
            io::output::loggers::simulation->info("Starting simulation");
            double current_time = start_time;
            int iteration = 0;
            // init forces
            calcF();
            // for this loop, we assume: current x, current f and current v are known
            while (current_time < end_time) {
                calcX();
                calcF();
                if(linkedCell) handleBounds();
                calcV();

                iteration++;
                if (iteration % 10 == 0) {
                    if(linkedCell) particleContainer.updateCells(); // update cell structure
                    writeParticle(particleContainer, outputFolder, outputBaseName, iteration);
                }
                if (iteration % 1000 == 0) {
                    io::output::loggers::simulation->info("Progress: {:03.2f}%", current_time / end_time * 100);
                    io::output::loggers::simulation->debug("Iteration {} finished.", iteration);
                }

                current_time += delta_t;
            }
        }

        /**
         * Runs the simulation and measures elapsed time. No output will be written during execution.
         * @param simIteration Amount of iterations the simulation should be performed, until average time is computed.
         * @param inputDataSource Name of where the starting data came from
         * @param startingData Data based on which the simulation should start
         * */
        void runBenchmark(const int simIteration, const std::string &inputDataSource,
                          const std::vector<Particle> &startingData) {
            std::chrono::high_resolution_clock::duration duration{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration minTime{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration maxTime{std::chrono::high_resolution_clock::duration::zero()};

            for (int pass{0}; pass < simIteration; pass++) {
                //reset data
                particleContainer = ParticleContainer(startingData);
                calcF.setParticleContainer(particleContainer);
                calcX.setParticleContainer(particleContainer);
                calcV.setParticleContainer(particleContainer);
                handleBounds.setParticleContainer(particleContainer);

                //get time stamp
                auto startTime = std::chrono::high_resolution_clock::now();

                //======================================
                double current_time = start_time;
                int iteration = 0;
                calcF();
                while (current_time < end_time) {
                    calcX();
                    calcF();
                    if(linkedCell) handleBounds();
                    calcV();
                    if (iteration % 10 == 0) {
                        if(linkedCell) particleContainer.updateCells();
                    }
                    current_time += delta_t;
                    iteration++;
                }
                //======================================

                //get timings
                auto endTime = std::chrono::high_resolution_clock::now();
                auto delta = endTime - startTime;
                if (delta < minTime) minTime = delta;
                if (delta > maxTime) maxTime = delta;
                duration += delta;

                io::output::loggers::simulation->debug("Finished pass {}", pass);
            }

            auto durationMillis =
                    std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / simIteration;
            auto minMillis = std::chrono::duration_cast<std::chrono::milliseconds>(minTime).count();
            auto maxMillis = std::chrono::duration_cast<std::chrono::milliseconds>(maxTime).count();

            io::output::loggers::simulation->info(
                    "###SimulationData:{}|Iterations:{}|AVG:{}|MIN:{}|MAX:{}|Particles:{}", inputDataSource,
                    simIteration, durationMillis, minMillis, maxMillis, startingData.size());
        }
    };
} // sim