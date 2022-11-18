/*
 * XYZWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "outputWriter/XYZWriter.h"
#include <iomanip>
#include <sstream>

namespace outputWriter
{

    XYZWriter::XYZWriter()
    {
        _memoryLogger = spdlog::get("memory_logger");
        _memoryLogger->info("XYZWriter generated!");
    }

    XYZWriter::~XYZWriter()
    {
        _memoryLogger->info("XYZWriter destructed!");
    }

    void XYZWriter::plotParticles(std::vector<Particle> particles,
                                  const std::string &filename, int iteration)
    {
        std::ofstream file;
        std::stringstream strstr;
        strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

        file.open(strstr.str().c_str());
        file << particles.size() << std::endl;
        file << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for "
                "file format doku."
             << std::endl;

        for (auto &p : particles)
        {
            std::array<double, 3> x = p.getX();
            file << "Ar ";
            file.setf(std::ios_base::showpoint);

            for (auto &xi : x)
            {
                file << xi << " ";
            }

            file << std::endl;
        }

        file.close();
    }

} // namespace outputWriter
