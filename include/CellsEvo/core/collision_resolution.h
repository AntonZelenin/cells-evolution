#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_

#include <vector>
#include <algorithm>
#include "world.h"

namespace cells_evo::collisions {
typedef std::pair<uint, uint> IdxPair;
typedef std::vector<IdxPair> IdXPairs;

struct FoodCellCollision {
  uint food_idx;
  uint cell_idx;
};

typedef std::vector<FoodCellCollision> FoodCellCollisions;

// todo implement more advanced collision resolver in future
class CollisionDetector {
 public:
  // todo it should be generic, accept iterator or something similar
  static IdXPairs DetectCellCollisions(core::CellStorage &cells);
  static FoodCellCollisions DetectCellFoodCollisions(core::CellStorage &cells, core::FoodStorage &foods);
  static std::optional<uint> FindClosestXFoodIdx(core::Cell &cell, core::FoodStorage &foods);
};

class CollisionResolver {
  constexpr static const float k_collision_distance_padding_ = 0.1;
 public:
  static void ResolveCollisions(core::Cell &cell_1, core::Cell &cell_2);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_
