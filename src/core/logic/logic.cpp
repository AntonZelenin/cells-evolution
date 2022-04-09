#include <vector>
#include <random>
#include "CellsEvo/core/logic/logic.h"
#include "CellsEvo/core/geometry.h"
#include "CellsEvo/core/collision_resolution.h"
#include "CellsEvo/core/logic/cell_logic.h"

namespace cells_evo::logic {
Logic::Logic(core::World &world, unsigned int food_production_rate) : world_(world) {
  food_production_rate_reverse_ = food_production_rate;
}

void Logic::WorldTick() {
  CountTick();
  auto colliding_cells = collisions::CollisionDetector::Detect(world_.cells_);

  colliding_cells = Eat(colliding_cells);
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
          reinterpret_cast<core::EdibleEntityStorage &>(world_.food_)
      );
    else
      hunter_cell_logic_.MoveCell(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage &>(world_.cells_)
      );
  }
}

void Logic::CheckCrossedBoundaries() {
  for (auto &[_, cell] : world_.cells_) {
    auto pos = cell->GetPosition();
    if (pos.X() < 0.0 + cell->GetRadius()) {
      pos.coordinates.x = cell->GetRadius();
    } else if (pos.X() > static_cast<float>(world_.width_) - cell->GetRadius()) {
      pos.coordinates.x = static_cast<float>(world_.width_) - cell->GetRadius();
    }
    if (pos.Y() < 0.0 + cell->GetRadius()) {
      pos.coordinates.y = cell->GetRadius();
    } else if (pos.Y() > static_cast<float>(world_.height_) - cell->GetRadius()) {
      pos.coordinates.y = static_cast<float>(world_.height_) - cell->GetRadius();
    }
    cell->SetPosition(pos);
  }
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
      new_cell->MoveX(cell->GetRadius() * 2);
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

// todo refactor
collisions::CellPtrPairs Logic::Eat(collisions::CellPtrPairs &colliding_cells) {
  for (auto &[_, cell] : world_.cells_) {
    if (cell->type_ == core::Type::K_NONHUNTER)
      // todo this method now is only used in one place, so it can be specific
      non_hunter_cell_logic_.ProcessEatFood(
          *cell,
          reinterpret_cast<core::EdibleEntityStorage &>(world_.food_)
      );
  }
  std::vector<unsigned int> eaten_cell_ids;
  std::vector<std::vector<collisions::CellPtrPair>::iterator> not_existing_pairs;
  for (
      auto colliding_cell_pair = colliding_cells.begin();
      colliding_cell_pair != colliding_cells.end();
      colliding_cell_pair++
      ) {
    if (HunterGotPrey(*colliding_cell_pair)) {
      auto prey_cell = ExtractPrey(*colliding_cell_pair);
      if (std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), prey_cell->GetId()) != eaten_cell_ids.end())
        continue;
      // todo it's duplicate
      ExtractHunter(*colliding_cell_pair)->AddEnergy(prey_cell->GetNutritionValue());
      eaten_cell_ids.push_back(prey_cell->GetId());
      not_existing_pairs.push_back(colliding_cell_pair);
      world_.cells_.erase(prey_cell->GetId());
    }
  }

  collisions::CellPtrPairs colliding_without_eaten;
  for (
      auto colliding_pair = colliding_cells.begin();
      colliding_pair != colliding_cells.end();
      colliding_pair++
      ) {
    if (std::find(not_existing_pairs.begin(), not_existing_pairs.end(), colliding_pair) == not_existing_pairs.end()) {
      colliding_without_eaten.push_back(*colliding_pair);
    }
  }

  return colliding_without_eaten;
}

bool Logic::HunterGotPrey(collisions::CellPtrPair &cell_pair) {
  // todo make method is hunter
  return (cell_pair.first->type_ == core::K_HUNTER && cell_pair.second->type_ != core::K_HUNTER)
      || (cell_pair.first->type_ != core::K_HUNTER && cell_pair.second->type_ == core::K_HUNTER);
}

std::shared_ptr<core::Cell> &Logic::ExtractHunter(collisions::CellPtrPair &cell_pair) {
  if (cell_pair.first->type_ == core::K_HUNTER)
    return cell_pair.first;
  else
    return cell_pair.second;
}

std::shared_ptr<core::Cell> &Logic::ExtractPrey(collisions::CellPtrPair &cell_pair) {
  if (cell_pair.first->type_ == core::K_NONHUNTER)
    return cell_pair.first;
  else
    return cell_pair.second;
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
}
}
