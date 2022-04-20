#include "CellsEvo/collision_resolution.h"

namespace cells_evo::collisions {
struct less_by_x {
  inline bool operator()(const std::weak_ptr<core::Entity> &entity_1, const std::weak_ptr<core::Entity> &entity_2) {
    return entity_1.lock()->GetPosition().x < entity_2.lock()->GetPosition().x;
  }
};

static bool CellsCollide(const std::shared_ptr<core::Entity> &entity_1, const std::shared_ptr<core::Entity> &entity_2) {
  return (entity_1->GetPosition() - entity_2->GetPosition()).MagnitudeSquared()
      < pow((entity_1->GetSize() + entity_2->GetSize()), 2);
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
      auto entity = entity_ptr->lock(), next_entity = (entity_ptr - 1)->lock();
      if (entity->GetEntityType() != core::EntityType::K_CELL
          || next_entity->GetEntityType() != core::EntityType::K_CELL)
        continue;
      if (CellsCollide(entity, next_entity)) {
        colliding_cells.push_back(
            {
                std::static_pointer_cast<core::Cell>(entity),
                std::static_pointer_cast<core::Cell>(next_entity)
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
    auto &[c_1, c_2] = pair;
    // todo make sure all ptr are not expired
    if (c_1.expired() || c_2.expired()) continue;
    auto cell_1 = c_1.lock(), cell_2 = c_2.lock();
    auto radius_sum = cell_1->GetSize() + cell_2->GetSize();
    auto diff_vector = cell_1->GetPosition() - cell_2->GetPosition();
    auto distance_to_move =
        radius_sum - diff_vector.Magnitude() + CollisionResolver::k_collision_distance_padding_;
    auto direction = diff_vector;
    direction.Normalize();

    cell_1->GetPosition() += direction * (distance_to_move * -1.0f * (cell_1->GetSize() / radius_sum));
    cell_2->GetPosition() += direction * (distance_to_move * (cell_2->GetSize() / radius_sum));
  }
}
}
