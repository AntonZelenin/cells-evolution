#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class NonHunterCellLogic {
  virtual std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      core::EdibleEntityStorage &foods
  ) {
    if (foods.empty())
      return nullptr;
    auto cached_closest_food = cell.GetFoodTargetId();
    if (cached_closest_food) {
      auto food = foods.find(cached_closest_food.value());
      if (food != foods.end())
        return food->second;
    }

    unsigned int closest_food_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, food] : foods) {
      auto dist = (cell.GetPosition() - food->GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_food_idx = idx;
      }
    }

    cell.SetFoodTargetId(closest_food_idx);
    return foods.find(closest_food_idx)->second;
  }

 public:
  virtual void MoveCell(
      core::Cell &cell,
      core::EdibleEntityStorage *food_entities
  ) {
    // check for new food every N frames
    if (cell.lifetime_ % 15 == 0) cell.ClearFoodTarget();
    auto direction = ChooseDirection(cell, *food_entities);
    Move(cell, direction, cell.GetSpeed());
  }

  void ProcessEatFood(
      core::Cell &cell,
      core::EdibleEntityStorage *food_entities
  ) {
    auto food = FindClosestFood(cell, *food_entities);
    if (food != nullptr && CellGotFood(cell, *food)) {
      cell.AddEnergy(food->GetNutritionValue());
      // todo this is not good
      food_entities->erase(food->GetId());
    }
  }

  // todo not duplicate?
  static bool CellGotFood(core::Cell &cell, core::Entity &food_entity);

  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
    return (cell.GetPosition() - food_entity.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  core::Vector2<float> ChooseDirection(core::Cell &cell, core::EdibleEntityStorage &food_entities) {
    // todo probably it should be generic, use for all types of cell
    core::Vector2<float> direction{};
    auto closest_food = FindClosestFood(cell, food_entities);
    if (!closest_food || !CouldSensedFood(cell, *closest_food)) {
      direction = GetRandomDirection(cell);
    } else {
      auto target_position = closest_food->GetPosition();
      direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    }
    return direction;
  }

  static core::Vector2<float> GetRandomDirection(core::Cell &cell);
};

class HunterCellLogic : public NonHunterCellLogic {
  std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      core::EdibleEntityStorage &cells
  ) override {
    if (cells.empty())
      return {};
    auto cached_closest_food = cell.GetFoodTargetId();
    if (cached_closest_food) {
      auto food = cells.find(cached_closest_food.value());
      if (food != cells.end())
        return food->second;
    }

    unsigned int closest_food_idx = 0;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, prey_cell] : reinterpret_cast<core::CellStorage&>(cells)) {
      if (prey_cell->type_ == core::K_HUNTER) continue;
      auto dist = (cell.GetPosition() - prey_cell->GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_food_idx = idx;
      }
    }
    if (closest_food_idx == 0) return {};
    cell.SetFoodTargetId(closest_food_idx);
    return cells.find(closest_food_idx)->second;
  }
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
