//
// Created by alex on 26.11.2022.
//

#pragma once

#include "ForceFunctorBase.h"

namespace sim::physics::force {

    /**
     * calculate the force for all particles using the Lennard-Jones potential, fast
     * */
    class FLennardJonesOMP : public ForceFunctorBase {
    private:
        pair_fun_t pairFun;

        void setPairFun();

    public:
        FLennardJonesOMP(double st,
                         double et,
                         double dt,
                         double eps,
                         double sig,
                         ParticleContainer &pc
        ) : ForceFunctorBase(st, et, dt, eps, sig, pc) {
            setPairFun();
        }

        void operator()() override;

        void setParticleContainer(ParticleContainer& pc) override;

        pair_fun_t& getForceFunction() override;
    };

} // sim::physics::force