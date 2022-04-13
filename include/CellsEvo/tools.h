#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_

#include <vector>
#include <random>
#include "geometry.h"

namespace cells_evo::core {
class IndexDriver {
  unsigned int idx_ = 1;
 public:
  unsigned int GetNextId();
};

class RandomEngine {
  std::mt19937 generator_;
  std::uniform_real_distribution<float> uniform_real_distribution_;

 public:
  RandomEngine() {
    std::random_device dev;
    generator_ = std::mt19937(dev());
  }

  float GetUniformReal(float from, float to) {
    return std::uniform_real_distribution<float>(from, to)(generator_);
  }

  std::vector<int> GetRandomInts(int from, int  to, unsigned int number) {
    std::vector<int> numbers{};
    std::uniform_int_distribution<int> distribution(from, to);

    for (; number > 0; number--) {
      numbers.push_back(distribution(generator_));
    }

    return numbers;
  }

  std::vector<float> GetRandomNormalFloats(float mean, float deviation, unsigned int number) {
    std::vector<float> numbers{};
    std::normal_distribution<float> distribution(mean, deviation);

    for (; number > 0; number--) {
      numbers.push_back(distribution(generator_));
    }

    return numbers;
  }

  std::vector<float> GetRandomFloats(float from, float  to, unsigned int number) {
    std::vector<float> numbers{};
    std::uniform_real_distribution<float> distribution(from, to);

    for (; number > 0; number--) {
      numbers.push_back(distribution(generator_));
    }

    return numbers;
  }
};

std::vector<Position> GenerateRandomPositions(
    int field_width,
    int field_height,
    int size,
    int min_distance_between_positions
);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_
