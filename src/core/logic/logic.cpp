#include <vector>
#include <random>
#include "CellsEvo/core/logic/logic.h"
#include "CellsEvo/core/geometry.h"
#include "CellsEvo/core/logic/cell_logic.h"

namespace cells_evo::logic {
Logic::Logic(core::World &world, unsigned int food_production_rate) : world_(world) {
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
  for (auto &[_, cell] : world_.cells_) {
    if (cell->type_ == core::Type::K_NONHUNTER)
      non_hunter_cell_logic_.MoveCell(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage *>(&world_.food_)
      );
    else
      hunter_cell_logic_.MoveCell(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage *>(&world_.cells_)
      );
  }
}

void Logic::CheckCrossedBoundaries() {
  for (auto &[_, cell] : world_.cells_) {
    CheckCellCrossedBoundaries(*cell);
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
    pos.coordinates.y = static_cast<float>(world_.height_) - cell.GetSize();
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
  }
}

void Logic::CheckCellsEnergy() {
  std::vector<unsigned int> cells_to_kill{};
  for (auto &[_, cell] : world_.cells_) {
    if (!cell->HasEnergy()) cells_to_kill.push_back(cell->id_);
  }
  for (auto cell_id : cells_to_kill) {
    world_.cells_.erase(cell_id);
  }
}

void Logic::DivideCells() {
  std::vector<std::shared_ptr<core::Cell>> new_cells;
  for (auto &[_, cell] : world_.cells_) {
    if (cell->HasEnergyToDivide()) {
      auto new_cell = DivideCell(*cell);
      new_cell->MoveX(cell->GetSize() * 2);
      new_cells.push_back(new_cell);
      cell->ConsumeDivisionEnergy();
    }
  }
  for (auto &cell : new_cells) {
    world_.AddCell(std::move(cell));
  }
}

// todo copy constructor?
std::shared_ptr<core::Cell> Logic::DivideCell(core::Cell &cell) {
  auto new_cell = std::make_shared<core::Cell>(
      cell.energy_ / 2,
      cell.type_,
      core::Position(cell.GetPosition().X(), cell.GetPosition().Y()),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  return std::move(new_cell);
}

void Logic::Eat() {
  for (auto &[_, cell] : world_.cells_) {
    if (cell->type_ == core::Type::K_NONHUNTER)
      non_hunter_cell_logic_.ProcessEatFood(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage *>(&world_.food_)
      );
    else
      hunter_cell_logic_.ProcessEatFood(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage *>(&world_.cells_)
      );
  }
}

void Move(core::Cell &cell, core::Vector2<float> &direction, float speed) {
  auto position = cell.GetPosition();
  // todo implement vector addition and multiplication
  auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
  cell.SetPosition(new_pos);
  cell.ConsumeMovementEnergy();
}

void Logic::CountTick() {
  for (auto &[_, cell] : world_.cells_) {
    if (cell->lifetime_ == std::numeric_limits<unsigned int>::max())
      cell->lifetime_ = 0;
    else
      cell->lifetime_++;
  }
  // todo remove?
  if (world_ticks_ == std::numeric_limits<unsigned int>::max())
    world_ticks_ = 0;
  else
    world_ticks_++;
}
}
