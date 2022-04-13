#include <vector>
#include "CellsEvo//logic/logic.h"
#include "CellsEvo/geometry.h"
#include "CellsEvo/collision_resolution.h"

namespace cells_evo::logic {
Logic::Logic(core::World &world, core::RandomGenerator random_generator)
    : world_(world), random_generator_(random_generator) {
}

void Logic::WorldTick() {
  CountTick();

  MoveCells();
  auto colliding_cells = collisions::CollisionDetector::Detect(world_.cells_);
  colliding_cells = Eat(colliding_cells);
  collisions::CollisionResolver::ResolveCollisions(colliding_cells);
  CheckCrossedBoundaries();
//  TeleportCrossedBoundaries();
  GenerateFood();
  DivideCells();
}

void Logic::MoveCells() {
  for (auto &[_, cell] : world_.cells_) {
    if (!cell->IsDead()) MoveCell(cell);
  }
}

void Logic::MoveCell(std::shared_ptr<core::Cell> &cell) {
  if (cell->IsNonHunter())
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

void Logic::TeleportCrossedBoundaries() {
  for (auto &[_, cell] : world_.cells_) {
    auto &pos = cell->GetPosition();
    if (pos.x < 0.0) {
      pos.x = static_cast<float>(world_.width_);
    } else if (pos.x > static_cast<float>(world_.width_)) {
      pos.x = 0.0;
    }
    if (pos.y < 0.0) {
      pos.y = static_cast<float>(world_.height_);
    } else if (pos.y > static_cast<float>(world_.height_)) {
      pos.y = 0.0;
    }
  }
}

// todo what if I make some static methods functions and remove them from headers?
void Logic::CheckCrossedBoundaries() {
  for (auto &[_, cell] : world_.cells_) {
    auto &pos = cell->GetPosition();
    bool hit_the_wall = false;

    if (pos.x < 0.0 + cell->GetSize()) {
      pos.x = cell->GetSize();
      hit_the_wall = true;
    } else if (pos.x > static_cast<float>(world_.width_) - cell->GetSize()) {
      pos.x = static_cast<float>(world_.width_) - cell->GetSize();
      hit_the_wall = true;
    }

    if (pos.y < 0.0 + cell->GetSize()) {
      pos.y = cell->GetSize();
      hit_the_wall = true;
    } else if (pos.y > static_cast<float>(world_.height_) - cell->GetSize()) {
      pos.y = static_cast<float>(world_.height_) - cell->GetSize();
      hit_the_wall = true;
    }

    if (hit_the_wall) cell->ClearDirection();
  }
}

void Logic::GenerateFood() {
  // generates food only once in N secs on average
  if (random_generator_.Get() <= 1) {
    world_.GenerateFood(1);
  }
}

void Logic::DivideCells() {
  std::vector<std::shared_ptr<core::Cell>> new_cells;
  for (auto &[_, cell] : world_.cells_) {
    if (cell->HasEnergyToDivide() && cell->DivisionCooldownPassed()) {
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
      cell.type_,
      core::Position(cell.GetPosition().x, cell.GetPosition().y),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  new_cell->AddEnergy(cell.energy_ / 2);
  new_cell->StartDivisionCooldown();
  cell.StartDivisionCooldown();
  return new_cell;
}

bool CanKill(std::shared_ptr<core::Cell> &hunter_cell, std::shared_ptr<core::Cell> &prey_cell) {
  return prey_cell->GetSize() < hunter_cell->GetSize() * 1.5;
}

// todo refactor
collisions::CellPtrPairs Logic::Eat(collisions::CellPtrPairs &colliding_cells) {
  for (auto &[_, cell] : world_.cells_) {
    if (cell->IsNonHunter())
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
    if (CanEat(*colliding_cell_pair)) {
      auto prey_cell = ExtractPrey(*colliding_cell_pair);
      auto hunter_cell = ExtractHunter(*colliding_cell_pair);
      if (std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), prey_cell->GetId()) != eaten_cell_ids.end())
        continue;
      if (!hunter_cell->IsHungry() || (!CanKill(hunter_cell, prey_cell) && !prey_cell->IsDead())) continue;
      // todo it's duplicate
      if (prey_cell->HasShell()) {
        prey_cell->DamageShell(hunter_cell->GetPunchStrength());
      } else {
        hunter_cell->AddEnergy(prey_cell->GetNutritionValue());
        eaten_cell_ids.push_back(prey_cell->GetId());
        not_existing_pairs.push_back(colliding_cell_pair);
        world_.cells_.erase(prey_cell->GetId());
      }
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

bool Logic::CanEat(collisions::CellPtrPair &cell_pair) {
  auto first = cell_pair.first, second = cell_pair.second;
  bool at_least_one_alive_hunter = (first->IsHunter() && !first->IsDead()) || (second->IsHunter() && !second->IsDead());
  bool one_dead = first->IsDead() || second->IsDead();

  if (at_least_one_alive_hunter && one_dead)
    return true;

  return (first->IsHunter() && second->IsNonHunter())
      || (first->IsNonHunter() && second->IsHunter());
}

std::shared_ptr<core::Cell> &Logic::ExtractHunter(collisions::CellPtrPair &cell_pair) {
  if (cell_pair.first->IsHunter() && !cell_pair.first->IsDead())
    return cell_pair.first;
  else
    return cell_pair.second;
}

std::shared_ptr<core::Cell> &Logic::ExtractPrey(collisions::CellPtrPair &cell_pair) {
  if (cell_pair.first->IsNonHunter() || cell_pair.first->IsDead())
    return cell_pair.first;
  else
    return cell_pair.second;
}

void Move(core::Cell &cell, core::Vector2<float> const &direction) {
  cell.GetPosition() += direction * cell.GetSpeed();
  cell.ConsumeMovementEnergy();
}

void Logic::CountTick() {
  for (auto &[_, cell] : world_.cells_) {
    cell->Tick();
  }
}
}
