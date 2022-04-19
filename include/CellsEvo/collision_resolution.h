#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_COLLISION_RESOLUTION_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_COLLISION_RESOLUTION_H_

#include <vector>
#include <algorithm>
#include "world.h"

namespace cells_evo::collisions {
typedef std::pair<std::weak_ptr<core::Cell>, std::weak_ptr<core::Cell>> CellPtrPair;
typedef std::vector<CellPtrPair> CellPtrPairs;

// todo implement more advanced collision resolver in future
class CollisionDetector {
 public:
  // todo it should be generic, accept iterator or something similar
//  static CellPtrPairs Detect(core::CellStorage &cells);
  static CellPtrPairs Detect(core::Field &field);
};

class CollisionResolver {
  constexpr static const float k_collision_distance_padding_ = 0.1;
 public:
  static void ResolveCollisions(CellPtrPairs &colliding_cells);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_COLLISION_RESOLUTION_H_
