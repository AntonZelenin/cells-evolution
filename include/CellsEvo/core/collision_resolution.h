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
    return entity_1->GetPosition().X() < entity_2->GetPosition().X();
  }
};

// todo implement more advanced collision resolver in future
class CollisionDetector {
 public:
  // todo it should be generic, accept iterator or something similar
  static CellPtrPairs Detect(core::CellStorage &cells) {
    std::vector<std::shared_ptr<core::Cell>> v_cells;
    for (auto &[_, cell] : cells) {
      v_cells.push_back(cell);
    }
    std::sort(v_cells.begin(), v_cells.end(), less_by_x());

    CellPtrPairs possibly_colliding_cells;
    for (auto cell_ptr = v_cells.begin() + 1; cell_ptr != v_cells.end(); cell_ptr++) {
      if (CellsOverlapByX(*cell_ptr, *(cell_ptr - 1))) {
        possibly_colliding_cells.push_back({*cell_ptr, *(cell_ptr - 1)});
      }
    }

    CellPtrPairs colliding_cells;
    for (auto&[cell_1, cell_2] : possibly_colliding_cells) {
      if (CellsCollide(cell_1, cell_2)) {
        colliding_cells.push_back({cell_1, cell_2});
      }
    }
    return colliding_cells;
  }

  static bool CellsOverlapByX(const std::shared_ptr<core::Cell> &cell_1, const std::shared_ptr<core::Cell> &cell_2) {
    return LeftEdgeX(cell_1) < RightEdgeX(cell_2);
  }

  static float LeftEdgeX(const std::shared_ptr<core::Cell> &cell_ptr) {
    return (*cell_ptr).GetPosition().X() - (*cell_ptr).GetRadius();
  }

  static float RightEdgeX(const std::shared_ptr<core::Cell> &cell_ptr) {
    return (*cell_ptr).GetPosition().X() + (*cell_ptr).GetRadius();
  }

  static bool CellsCollide(const std::shared_ptr<core::Cell> &cell_1, const std::shared_ptr<core::Cell> &cell_2) {
    return (cell_1->GetPosition() - cell_2->GetPosition()).Magnitude() < (cell_1->GetRadius() + cell_2->GetRadius());
  }
};

class CollisionResolver {

};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_COLLISION_RESOLUTION_H_
