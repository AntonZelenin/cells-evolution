#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {

// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class NonHunterLogic {
  std::map<unsigned int, unsigned int> cell_food_cache_;
 public:
  void MoveCells(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food,
      unsigned int world_tick
  ) {
    for (auto&[_, cell] : cells) {
      auto direction = ChooseDirection(cell, food);
      Move(cell, direction, cell.speed_);
    }
  };

  core::Vector2<float> ChooseDirection(core::Cell &cell, std::map<unsigned int, core::Food> &food) {
    core::Vector2<float> direction{};
    auto closest_food = FindClosestFood(cell, food);
    if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
      direction = GetRandomSinDirection();
    } else {
      auto target_position = closest_food.value().GetPosition();
      direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    }
    return direction;
  }

  // todo I want it to return a reference, do I?
  std::optional<core::Food> FindClosestFood(core::Cell &cell, std::map<unsigned int, core::Food> &foods) {
    if (foods.empty())
      return {};
    auto cached_closest_food = cell_food_cache_.find(cell.id_);
    if (cached_closest_food != cell_food_cache_.end()) {
      auto food_id = cached_closest_food->second;
      auto result = foods.find(food_id);
      if (result != foods.end())
        return result->second;
      else
        cell_food_cache_.erase(food_id);
    }

    unsigned int closest_food_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, food] : foods) {
      // todo seems position makes life harder
      auto dist = (food.GetPosition() - cell.GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_food_idx = idx;
      }
    }

    cell_food_cache_.insert({cell.id_, closest_food_idx});
    return foods.find(closest_food_idx)->second;
  }

  static bool CouldSensedFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  core::Vector2<float> GetRandomSinDirection() {
    // todo here
    return {1.0, 0.5};
  }

  void RebuildCellsFoodCache(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  ) {
    cell_food_cache_.clear();
    for (auto&[_, cell] : cells) {
      FindClosestFood(cell, food);
    }
  }

  void ProcessEatFood(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  ) {
    for (auto&[_, cell] : cells) {
      auto closest_food = FindClosestFood(cell, food);
      if (closest_food && CellGotFood(cell, closest_food.value())) {
        cell.AddEnergy(core::Food::k_default_energy_value_);
        food.erase(closest_food.value().id_);
      }
    }
  }

  static bool CellGotFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() < cell.size_;
  }
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
