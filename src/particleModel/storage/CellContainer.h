#pragma once


#include "utils/ForceCalculations.h"
#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <vector>

typedef int dim_t;
extern dim_t dim_t_res;

class CellContainer {
public:
    CellContainer(double domain_width, double domain_height, double domain_depth, double r_cutoff, double cell_size, const std::string& forceType);

    virtual ~CellContainer();

    void setNextCell(std::array<dim_t, 3> &next_position);

    void setNextPath(std::array<dim_t, 3> &start, std::array<dim_t, 3> &pattern);

    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg);

    void plotParticles(outputWriter::VTKWriter &writer);

    std::string to_string();

    size_t size();

    auto getParticles() {
        return &particles;
    }

private:
    bool three_dimensions;
    double cell_size;
    std::array<dim_t, 3> domain_max;
    std::array<double, 3> domain_borders;
    dim_t comparing_depth = 1;
    size_t particle_amount = 0;
    ForceCalculation forceLambda;
    std::vector<std::vector<std::vector<std::vector<Particle>>>> particles;

    void setNext3dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void setNext2dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);
};

