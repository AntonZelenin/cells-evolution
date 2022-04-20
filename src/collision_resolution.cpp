#include "CellsEvo/collision_resolution.h"

namespace cells_evo::collisions {
struct less_by_x {
  inline bool operator()(const std::weak_ptr<core::Entity> &entity_1, const std::weak_ptr<core::Entity> &entity_2) {
    return entity_1.lock()->GetPosition().x < entity_2.lock()->GetPosition().x;
  }
};

static bool CellsCollide(const std::weak_ptr<core::Entity> &entity_1, const std::weak_ptr<core::Entity> &entity_2) {
  return (entity_1.lock()->GetPosition() - entity_2.lock()->GetPosition()).MagnitudeSquared()
      < pow((entity_1.lock()->GetSize() + entity_2.lock()->GetSize()), 2);
}

//CellPtrPairs CollisionDetector::Detect(core::CellStorage &cells) {
//  if (cells.size() < 2)
//    return {};
//
//  std::vector<std::shared_ptr<core::Cell>> v_cells;
//  for (auto &[_, cell] : cells) {
//    v_cells.push_back(cell);
//  }
//  std::sort(v_cells.begin(), v_cells.end(), less_by_x());
//
//  CellPtrPairs colliding_cells;
//  for (auto cell_ptr = v_cells.begin() + 1; cell_ptr != v_cells.end(); cell_ptr++) {
//    if (CellsCollide(*cell_ptr, *(cell_ptr - 1))) {
//      colliding_cells.push_back({*cell_ptr, *(cell_ptr - 1)});
//    }
//  }
//
//  return colliding_cells;
//}

CellPtrPairs CollisionDetector::Detect(core::Field &field) {
  CellPtrPairs colliding_cells;
  for (auto &tile : field.tiles_) {
    if (tile.size() < 2)
      continue;

    std::sort(tile.begin(), tile.end(), less_by_x());

    for (auto entity_ptr = tile.begin() + 1; entity_ptr != tile.end(); entity_ptr++) {
//      if (entity_ptr->lock()->GetEntityType() != core::EntityType::K_CELL
//          || (entity_ptr - 1)->lock()->GetEntityType() != core::EntityType::K_CELL)
//        continue;
      if (CellsCollide(*entity_ptr, *(entity_ptr - 1))) {
        colliding_cells.push_back(
            {
                std::static_pointer_cast<core::Cell>(entity_ptr->lock()),
                std::static_pointer_cast<core::Cell>((entity_ptr - 1)->lock())
            }
        );
      }
    }
  }

  return colliding_cells;
}

void CollisionResolver::ResolveCollisions(CellPtrPairs &colliding_cells) {
// it assumes there are no pairs of hunter-nonhunter
  for (auto &pair : colliding_cells) {
    auto &[cell_1, cell_2] = pair;
    // todo make sure all ptr are not expired
    if (cell_1.expired() || cell_2.expired()) continue;
    auto radius_sum = cell_1.lock()->GetSize() + cell_2.lock()->GetSize();
    auto diff_vector = cell_1.lock()->GetPosition() - cell_2.lock()->GetPosition();
    auto distance_to_move =
        radius_sum - diff_vector.Magnitude() + CollisionResolver::k_collision_distance_padding_;
    auto direction = diff_vector;
    direction.Normalize();

    cell_1.lock()->GetPosition() += direction * (distance_to_move * -1.0f * (cell_1.lock()->GetSize() / radius_sum));
    cell_2.lock()->GetPosition() += direction * (distance_to_move * (cell_2.lock()->GetSize() / radius_sum));
  }
}
}
