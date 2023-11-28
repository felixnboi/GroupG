#pragma once

#include <string>

#include "io/output/vtk/VTKWriter.h"
#include "io/output/xyz/XYZWriter.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Wrapper class to abstract the writing of output files
 *
 * Currently there are two supported output formats: VTK and XYZ. Additionally a 'NONE' format is available, which does not write any
 * output.
 */
class FileOutputHandler {
   public:
    enum class OutputFormat { VTK, XYZ, NONE };

   private:
    const OutputFormat output_format;
    const std::string output_dir_path;

   public:
    /**
     * @brief Construct a new FileOutputHandler object
     *
     * @param output_format The format of the output files
     * @param output_dir_path The path to the directory in which to save the output
     */
    FileOutputHandler(const OutputFormat output_format, const std::string& output_dir_path = "./output");

    /**
     * @brief Writes the given std::unique_ptr<ParticleContainer>& to a file
     *
     * @param iteration The current iteration of the simulation
     * @param particle_container The std::unique_ptr<ParticleContainer>& to write to the file
     */
    void writeFile(int iteration, const std::unique_ptr<ParticleContainer>& particle_container) const;
};