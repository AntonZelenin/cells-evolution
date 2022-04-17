#include <utility>
#include <vector>
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/collision_resolution.h"

namespace cells_evo::logic {
Logic::Logic(core::World &world, float food_production_rate)
    : world_(world) {
  food_production_rate_ = food_production_rate;
}

void Logic::WorldTick() {
  CountTick();

  MoveCells();
  auto colliding_cells = collisions::CollisionDetector::Detect(world_.cells_);
  colliding_cells = Eat(colliding_cells);
  collisions::CollisionResolver::ResolveCollisions(colliding_cells);
//  TeleportCrossedBoundaries();
  GenerateFood();
  UpdateCellsState();
}

void Logic::MoveCells() {
  for (auto &[_, cell] : world_.cells_) {
    if (!cell->IsDead()) {
      MoveCell(cell);
      CheckCrossedBoundaries(cell);
    }
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

[[maybe_unused]] void Logic::TeleportCrossedBoundaries(std::shared_ptr<core::Cell> &cell) const {
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

// todo what if I make some static methods functions and remove them from headers?
void Logic::CheckCrossedBoundaries(std::shared_ptr<core::Cell> &cell) const {
  auto &pos = cell->GetPosition();
  bool hit_the_wall = false;

//  if (pos.x < cell->GetSize()) {
  if (pos.x < 0.1) {
//    pos.x = cell->GetSize();
    pos.x = 0.1;
    hit_the_wall = true;
//  } else if (pos.x > static_cast<float>(world_.width_) - cell->GetSize()) {
  } else if (pos.x > static_cast<float>(world_.width_) - 0.1) {
//    pos.x = static_cast<float>(world_.width_) - cell->GetSize();
    pos.x = static_cast<float>(world_.width_) - 0.1;
    hit_the_wall = true;
  }

//  if (pos.y < cell->GetSize()) {
  if (pos.y < 0.1) {
//    pos.y = cell->GetSize();
    pos.y = 0.1;
    hit_the_wall = true;
//  } else if (pos.y > static_cast<float>(world_.height_) - cell->GetSize()) {
  } else if (pos.y > static_cast<float>(world_.height_) - 0.1) {
//    pos.y = static_cast<float>(world_.height_) - cell->GetSize();
    pos.y = static_cast<float>(world_.height_) - 0.1;
    hit_the_wall = true;
  }

  if (hit_the_wall) cell->ClearDirection();
}

void Logic::GenerateFood() {
  if (ShouldGenerateFood()) {
    world_.GenerateFood(1);
  }
}

bool Logic::ShouldGenerateFood() {
  // generates food only once in N secs on average
  return random_generator_.GetUniformReal(0, food_production_rate_) <= 1;
}

void Logic::UpdateCellsState() {
  std::vector<std::shared_ptr<core::Cell>> new_cells;
  for (auto pair = world_.cells_.begin(); pair != world_.cells_.end();) {
    auto &cell = pair->second;
    if (cell->HasEnergyToDivide() && cell->DivisionCooldownPassed()) {
      new_cells.push_back(DivideCell(*cell));
      pair++;
    } else if (cell->HasDecayed()) {
      world_.AddFood(
          std::make_shared<core::Food>(core::Food(
              core::FoodType::K_FLORAL,
              cell->GetPosition(),
              cell->GetBaseNutritionValue()
          ))
      );
      world_.cells_.erase(pair++);
    } else {
      pair++;
    }
  }
  for (auto &cell : new_cells) {
    world_.AddCell(std::move(cell));
  }
}

std::shared_ptr<core::Cell> Logic::DivideCell(core::Cell &cell) {
  auto new_cell = std::make_shared<core::Cell>(
      cell.type_,
      cell.GetPosition(),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  new_cell->AddEnergy(cell.energy_ / 2);
  new_cell->StartDivisionCooldown();
  new_cell->MoveX(cell.GetSize() * 2);
  cell.StartDivisionCooldown();
  cell.ConsumeDivisionEnergy();
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
        hunter_cell->ConsumePunchEnergy();
      } else {
        hunter_cell->AddEnergy(prey_cell->GetNutritionValue());
        hunter_cell->ClearFoodTarget();
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

  if (!at_least_one_alive_hunter)
    return false;

  if (first->IsDead() || second->IsDead())
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

void Logic::CountTick() {
  for (auto &[_, cell] : world_.cells_) {
    cell->Tick();
  }
}
}
