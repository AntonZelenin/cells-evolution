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

std::vector<int> GetRandomInts(int from, int  to, unsigned int number);
std::vector<float> GetRandomFloats(float from, float  to, unsigned int number);
std::vector<float> GetRandomNormalFloats(float mean, float deviation, unsigned int number);

std::vector<Position> GenerateRandomPositions(
    int field_width,
    int field_height,
    int size,
    int min_distance_between_positions
);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_
