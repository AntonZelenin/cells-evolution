#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"
#include <vector>
#include <random>

namespace cells_evo::logic {
Logic::Logic(core::World &world, unsigned int food_production_rate) : world_(world) {
  non_hunter_logic_.RebuildCellsFoodCache(world_.cells_, world_.food_);
  food_production_rate_ = food_production_rate;
}

void Logic::WorldTick() {
  CountTick();

  Eat();
  non_hunter_logic_.MoveCells(world_.cells_, world_.food_, world_ticks_);
  hunter_logic_.MoveCells(world_.hunter_cells_, world_.cells_, world_ticks_);
  CheckCellsEnergy();
  GenerateFood();
  DivideCells();
}

void Logic::GenerateFood() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<unsigned int> distribution(0, food_production_rate_);
  // generate food only once in N secs on average
  if (distribution(generator) == 1) {
    world_.GenerateFood(1);
    non_hunter_logic_.RebuildCellsFoodCache(world_.cells_, world_.food_);
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
    world_.cells_.erase(cell_id);
  }
  for (auto cell_id : hunter_cells_to_kill) {
    world_.cells_.erase(cell_id);
  }
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

void Logic::Eat() {
  non_hunter_logic_.ProcessEatFood(world_.cells_, world_.food_);
  hunter_logic_.ProcessEatCell(world_.hunter_cells_, world_.cells_);
}

void Move(core::Cell &cell, core::Vector2<float> &direction, float speed) {
  auto position = cell.GetPosition();
  // todo implement vector addition and multiplication
  auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
  cell.SetPosition(new_pos);
  cell.ConsumeMovementEnergy();
}

void Logic::CountTick() {
  if (world_ticks_ == std::numeric_limits<unsigned int>::max())
    world_ticks_ = 0;
  else
    world_ticks_++;
}
}
