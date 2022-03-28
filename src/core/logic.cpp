#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"

#include <vector>
#include <random>

namespace cells_evo::logic {
Logic::Logic(core::World &world) : world_(world) {
  RebuildCellsFoodCache();
}

void Logic::WorldTick() {
  CountTick();

  ProcessEvents();
  MoveCells();
  MoveHunterCells();
  CheckCellsEnergy();
  GenerateFood();
  DivideCells();
}

void Logic::GenerateFood() {
  double secs = 2;
  // todo this is not good
  int frames = static_cast<int>(secs / 0.016666);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, frames);
  // generate food only once in N secs on average
  if (distribution(generator) == 1) {
    world_.GenerateFood(1);
    RebuildCellsFoodCache();
  }
}

void Logic::CheckCellsEnergy() {
  std::vector<unsigned int> cells_to_kill{}, hunter_cells_to_kill{};
  for (auto &[_, cell] : world_.cells_) {
    if (!cell.HasEnergy()) cells_to_kill.push_back(cell.id_);
  }
  // todo I guess I should keep hunter cells and cells together
  for (auto &[_, hunter_cell] : world_.hunter_cells_) {
    if (!hunter_cell.HasEnergy()) hunter_cells_to_kill.push_back(hunter_cell.id_);
  }
  for (auto cell_id : cells_to_kill) {
    KillCell(cell_id);
  }
  for (auto cell_id : hunter_cells_to_kill) {
    KillCell(cell_id);
  }
}

void Logic::KillCell(unsigned int cell_id) {
  cell_food_cache_.erase(cell_id);
  std::vector<unsigned int> hunter_cells_to_remove{};
  for (auto[hunter_cell_id, prey_cell_id] : hunter_cell_cell_cache_) {
    if (prey_cell_id == cell_id) hunter_cells_to_remove.push_back(hunter_cell_id);
  }
  for (auto hunter_cell_id : hunter_cells_to_remove) {
    hunter_cell_cell_cache_.erase(hunter_cell_id);
  }
  world_.cells_.erase(cell_id);
}

void Logic::DivideCells() {
  for (auto &[_, cell] : world_.cells_) {
    if (cell.HasEnergyToDivide()) {
      core::Cell new_cell = DivideCell(cell);
      new_cell.MoveX(cell.size_ * 2);
      world_.AddCell(new_cell);
      cell.ConsumeDivisionEnergy();
    }
  }
  // todo duplicate
  for (auto &[_, hunter_cell] : world_.hunter_cells_) {
    if (hunter_cell.HasEnergyToDivide()) {
      core::Cell new_cell = DivideCell(hunter_cell);
      new_cell.MoveX(hunter_cell.size_ * 2);
      world_.AddHunterCell(new_cell);
      hunter_cell.ConsumeDivisionEnergy();
    }
  }
}

// todo copy constructor?
core::Cell Logic::DivideCell(core::Cell &cell) {
  return {
      cell.size_,
      cell.speed_,
      cell.energy_ / 2,
      cell.type_,
      core::Position(cell.GetPosition().X(), cell.GetPosition().Y())
  };
}

void Logic::RebuildCellsFoodCache() {
  cell_food_cache_.clear();
  for (auto&[_, cell] : world_.cells_) {
    FindClosestFood(cell);
  }
}

void Logic::ProcessEvents() {
  for (auto&[_, cell] : world_.cells_) {
    ProcessEatFood(cell);
  }
  for (auto&[_, hunter_cell] : world_.hunter_cells_) {
    ProcessEatCell(hunter_cell);
  }
}

void Logic::ProcessEatFood(core::Cell &cell) {
  auto food = FindClosestFood(cell);
  if (food && CellGotFood(cell, food.value())) {
    cell.AddEnergy(core::Food::k_default_energy_value_);
    // todo separate method like KillCell?
    RemoveFoodFromCache(food.value());
    world_.food_.erase(food.value().id_);
  }
}

void Logic::ProcessEatCell(core::Cell &hunter_cell) {
  auto cell = FindClosestCell(hunter_cell);
  if (cell && HunterCellGotCell(hunter_cell, cell.value())) {
    hunter_cell.AddEnergy(core::Cell::k_default_energy_value_);
    KillCell(cell.value().id_);
  }
}

void Logic::RemoveFoodFromCache(const core::Food &food) {
  std::vector<unsigned int> cells_to_remove{};
  for (auto[cell_id, food_id] : cell_food_cache_) {
    if (food.id_ == food_id) cells_to_remove.push_back(cell_id);
  }
  for (auto cell_id : cells_to_remove) {
    cell_food_cache_.erase(cell_id);
  }
}

bool Logic::CellGotFood(core::Cell &cell, core::Food &food) {
  return (cell.GetPosition() - food.GetPosition()).Magnitude() < cell.size_;
}

// todo not duplicate?
bool Logic::HunterCellGotCell(core::Cell &hunter_cell, core::Cell &cell) {
  return (hunter_cell.GetPosition() - cell.GetPosition()).Magnitude() < (hunter_cell.size_ + cell.size_);
}

void Move(core::Cell &cell, core::Vector2<float> &direction, float speed) {
  auto position = cell.GetPosition();
  // todo implement vector addition and multiplication
  auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
  cell.SetPosition(new_pos);
  cell.ConsumeMovementEnergy();
}

void Logic::MoveCells() {
  for (auto&[_, cell] : world_.cells_) {
    auto closest_food = FindClosestFood(cell);
    core::Vector2<float> direction{};
    if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
      direction = GetRandomSinDirection();
    } else {
      auto target_position = closest_food.value().GetPosition();
      direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    }
    Move(cell, direction, cell.speed_);
  }
}

bool Logic::CouldSensedFood(core::Cell &cell, core::Food &food) {
  return (cell.GetPosition() - food.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
}

core::Vector2<float> Logic::GetRandomSinDirection() {
  return {1.0, 0.5};
}

void Logic::MoveHunterCells() {
  // update once in 10 frames
  if (world_ticks_ % 10 == 0) hunter_cell_cell_cache_.clear();
  for (auto&[_, cell] : world_.hunter_cells_) {
    auto closest_cell = FindClosestCell(cell);
    if (!closest_cell) return;
    auto target_position = closest_cell.value().GetPosition();
    auto direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    Move(cell, direction, cell.speed_);
  }
}

// todo I want it to return a reference
std::optional<core::Food> Logic::FindClosestFood(core::Cell &cell) {
  if (world_.food_.empty())
    return {};
  if (cell_food_cache_.find(cell.id_) != cell_food_cache_.end())
    return world_.food_.find(cell_food_cache_.find(cell.id_)->second)->second;

  unsigned int closest_food_idx;
  float min_distance = std::numeric_limits<float>::max();
  for (auto&[idx, food] : world_.food_) {
    // todo seems position makes life harder
    auto dist = (food.GetPosition() - cell.GetPosition()).Magnitude();
    if (dist < min_distance) {
      min_distance = dist;
      closest_food_idx = idx;
    }
  }

  cell_food_cache_.insert({cell.id_, closest_food_idx});
  return world_.food_.find(closest_food_idx)->second;
}

// todo looks like duplicate
std::optional<core::Cell> Logic::FindClosestCell(core::Cell &hunter_cell) {
  if (world_.cells_.empty())
    return {};
  if (hunter_cell_cell_cache_.find(hunter_cell.id_) != hunter_cell_cell_cache_.end())
    return world_.cells_.find(hunter_cell_cell_cache_.find(hunter_cell.id_)->second)->second;

  unsigned int closest_cell_idx;
  float min_distance = std::numeric_limits<float>::max();
  for (auto&[idx, cell] : world_.cells_) {
    auto dist = (cell.GetPosition() - hunter_cell.GetPosition()).Magnitude();
    if (dist < min_distance) {
      min_distance = dist;
      closest_cell_idx = idx;
    }
  }

  hunter_cell_cell_cache_.insert({hunter_cell.id_, closest_cell_idx});
  return world_.cells_.find(closest_cell_idx)->second;
}

void Logic::CountTick() {
  if (world_ticks_ == std::numeric_limits<unsigned int>::max())
    world_ticks_ = 0;
  else
    world_ticks_++;
}
}
