#include <vector>
#include <random>
#include "CellsEvo/core/logic/logic.h"
#include "CellsEvo/core/geometry.h"
#include "CellsEvo/core/logic/cell_logic.h"

namespace cells_evo::logic {
Logic::Logic(core::World &world, unsigned int food_production_rate) : world_(world) {
  cell_logic_.RebuildCellsFoodCache(world_.cells_, world_.food_);
  food_production_rate_reverse_ = food_production_rate;
}

void Logic::WorldTick() {
  CountTick();

  Eat();
  MoveCells();
  CheckCrossedBoundaries();
  CheckCellsEnergy();
  GenerateFood();
  DivideCells();
}

void Logic::MoveCells() {
  cell_logic_.MoveCells(world_.cells_, world_.food_, world_ticks_);
  cell_logic_.MoveCells(world_.hunter_cells_, world_.cells_, world_ticks_);
}

void Logic::CheckCrossedBoundaries() {
  for (auto &[_, cell]: world_.cells_) {
    CheckCellCrossedBoundaries(cell);
  }
  for (auto &[_, cell]: world_.hunter_cells_) {
    CheckCellCrossedBoundaries(cell);
  }
}

void Logic::CheckCellCrossedBoundaries(core::Cell &cell) const {
  // todo this is slow
  auto pos = cell.GetPosition();
  if (pos.X() < 0.0 + cell.GetSize()) {
    pos.coordinates.x = cell.GetSize();
  } else if (pos.X() > static_cast<float>(world_.width_) - cell.GetSize()) {
    pos.coordinates.x = static_cast<float>(world_.width_) - cell.GetSize();
  }
  if (pos.Y() < 0.0 + cell.GetSize()) {
    pos.coordinates.y = cell.GetSize();
  } else if (pos.Y() > static_cast<float>(world_.height_) - cell.GetSize()) {
    pos.coordinates.x = static_cast<float>(world_.height_) - cell.GetSize();
  }
  cell.SetPosition(pos);
}

void Logic::GenerateFood() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<unsigned int> distribution(0, food_production_rate_reverse_);
  // generate food only once in N secs on average
  if (food_production_rate_reverse_ != 0 && distribution(generator) == 1) {
    world_.GenerateFood(1);
    cell_logic_.RebuildCellsFoodCache(world_.cells_, world_.food_);
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
    world_.hunter_cells_.erase(cell_id);
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
  core::Cell new_cell{
      cell.size_,
      cell.speed_,
      cell.energy_ / 2,
      cell.type_,
      core::Position(cell.GetPosition().X(), cell.GetPosition().Y()),
      genetic_engineer_.CopyGenes(cell.genes_)
  };
  return new_cell;
}

void Logic::Eat() {
  cell_logic_.ProcessEatFood<core::Food>(world_.cells_, world_.food_);
  cell_logic_.ProcessEatFood<core::Cell>(world_.hunter_cells_, world_.cells_);
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
