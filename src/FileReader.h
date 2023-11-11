/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Particle.h"
#include "ParticleContainer.h"

class FileReader {
 public:
  FileReader();
  virtual ~FileReader();

  /// if set FileReader shows debug output
  static const bool verbose_FileReader = false;

  /**
   * several
   *
   */
  struct CuboidData {
    /// inital velocity and position vectors
    std::array<double, 3> x, v;

    /// N1: amount of particles along dimension 1
    /// N2: amount of particles along dimension 2
    /// N3: amount of particles  along dimension 3
    uint64_t N1, N2, N3;

    /// Mass m of the particles in the cuboid
    /// Mesh width h
    double m, h;

    /// sigma and epsilon parameters for the froce calculation
    /// between particles of this cuboid
    double sigma, epsilon;

    /// Average velocity (always 0.1 in our case)
    double avg_v = 0.1;

    /**
     * @brief Convert CuboidData to a string
     *
     * @return String representation of CuboidData
     */
    std::string to_string() const {
      std::stringstream ss;

      ss << "CuboidData:" << std::endl;
      ss << "  x: (" << x[0] << ", " << x[1] << ", " << x[2] << ")"
         << std::endl;
      ss << "  v: (" << v[0] << ", " << v[1] << ", " << v[2] << ")"
         << std::endl;
      ss << "  N1: " << N1 << std::endl;
      ss << "  N2: " << N2 << std::endl;
      ss << "  N3: " << N3 << std::endl;
      ss << "  m: " << m << std::endl;
      ss << "  h: " << h << std::endl;
      ss << "  sigma: " << sigma << std::endl;
      ss << "  epsilon: " << epsilon << std::endl;

      return ss.str();
    }
  };

  /**
   * @brief Reads particle data from a file and adds them into a
   * ParticleContainer
   *
   * reads particle data from a the file specified by filename(file has to have
   * specific format) and adds them inplace into the particleContainer
   *
   *
   * @param particleContainer reference to the ParticleContainer to add to
   * @param filename Filename of the file containing particle data
   */
  void readParticleFile(ParticleContainer &particleContainer, char *filename);

  /**
   * @brief Reads Cuboids of Particles from a file and returns a list of
   * CuboidData structs
   *
   * reads cuboids from a file specified by the given filename(file has to have
   * specific format). A cuboid in the file is just a list of the parameters
   * that are in the CuboidData struct. Turns the parameters given in the file
   * into a struct containing the parameters and returns a list of CuboidData
   * structs based on the read data.
   *
   * @param filename Filename of the file containing CuboidData
   * @return List of CuboidData structs read from the file
   */
  std::list<FileReader::CuboidData> readCuboidFile(char *filename);
};
