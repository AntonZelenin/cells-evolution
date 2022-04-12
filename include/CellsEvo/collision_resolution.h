#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_

#include <vector>
#include <algorithm>
#include "world.h"

namespace cells_evo::collisions {
typedef std::pair<std::shared_ptr<core::Cell>, std::shared_ptr<core::Cell>> CellPtrPair;
typedef std::vector<CellPtrPair> CellPtrPairs;

struct less_by_x {
  inline bool operator()(const std::shared_ptr<core::Entity> &entity_1, const std::shared_ptr<core::Entity> &entity_2) {
    return entity_1->GetPosition().x < entity_2->GetPosition().x;
  }
};

// todo implement more advanced collision resolver in future
class CollisionDetector {
 public:
  // todo it should be generic, accept iterator or something similar
  static CellPtrPairs Detect(core::CellStorage &cells);
};

class CollisionResolver {
  constexpr static const float k_collision_distance_dont_remember_ = 0.3;
 public:
  static void ResolveCollisions(CellPtrPairs &colliding_cells);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_