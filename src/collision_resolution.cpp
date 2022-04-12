#include "CellsEvo/collision_resolution.h"

namespace cells_evo::collisions {
static float LeftEdgeX(const std::shared_ptr<core::Cell> &cell_ptr) {
  return (*cell_ptr).GetPosition().x - (*cell_ptr).GetRadius();
}

static float RightEdgeX(const std::shared_ptr<core::Cell> &cell_ptr) {
  return (*cell_ptr).GetPosition().x + (*cell_ptr).GetRadius();
}

static bool CellsOverlapByX(const std::shared_ptr<core::Cell> &cell_1, const std::shared_ptr<core::Cell> &cell_2) {
  return LeftEdgeX(cell_1) < RightEdgeX(cell_2);
}

static bool CellsCollide(const std::shared_ptr<core::Cell> &cell_1, const std::shared_ptr<core::Cell> &cell_2) {
  return (cell_1->GetPosition() - cell_2->GetPosition()).Magnitude() < (cell_1->GetRadius() + cell_2->GetRadius());
}

cells_evo::collisions::CellPtrPairs cells_evo::collisions::CollisionDetector::Detect(cells_evo::core::CellStorage &cells) {
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
void CollisionResolver::ResolveCollisions(CellPtrPairs &colliding_cells) {
// it assumes there are no pairs of hunter-nonhunter
  for (auto &pair : colliding_cells) {
    // todo does & make difference?
    auto &[cell_1, cell_2] = pair;
    auto radius_sum = cell_1->GetRadius() + cell_2->GetRadius();
    auto diff_vector = cell_1->GetPosition() - cell_2->GetPosition();
    auto distance_to_move =
        radius_sum - diff_vector.Magnitude() + CollisionResolver::k_collision_distance_dont_remember_;
    auto direction = diff_vector;
    direction.Normalize();

    cell_1->GetPosition() += direction * (distance_to_move * -1.0f * (cell_1->GetRadius() / radius_sum));
    cell_2->GetPosition() += direction * (distance_to_move * (cell_2->GetRadius() / radius_sum));
  }
}
}