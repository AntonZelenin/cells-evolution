#include <vector>
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/collision_resolution.h"
#include "SFML/System/Clock.hpp"

namespace cells_evo::logic {
Logic::Logic(core::World &world, float food_production_rate)
    : world_(world) {
  food_production_rate_ = food_production_rate;
}

void Logic::WorldTick() {
  // todo gui counts deleted cells
  sf::Clock clock;
  Tick();
  ChooseDirections();
  auto dir = clock.restart().asMicroseconds();
  MoveCells();
  auto move = clock.restart().asMicroseconds();
  if (world_.ticks_ % 2 == 0) {
    auto clean_food = clock.restart().asMicroseconds();
    world_.SortFood();
    auto sort_food = clock.restart().asMicroseconds();
    world_.SortCells();
    auto sort_cells = clock.restart().asMicroseconds();
    auto colliding_cell_ids = collisions::CollisionDetector::DetectCellCollisions(world_.cells_);
    auto coll_cell = clock.restart().asMicroseconds();
    auto colliding_cell_food_ids = collisions::CollisionDetector::DetectCellFoodCollisions(world_.cells_, world_.food_);
    auto coll_food = clock.restart().asMicroseconds();
    HunterEat(colliding_cell_ids);
    auto hunter_eat = clock.restart().asMicroseconds();
    NonHunterEat(colliding_cell_food_ids);
    auto nonhunter_eat = clock.restart().asMicroseconds();
    ResolveCellCollisions(colliding_cell_ids);
    auto res_coll = clock.restart().asMicroseconds();
  }
//  TeleportCrossedBoundaries();
  GenerateFood();
  auto generate_food = clock.restart().asMicroseconds();
  UpdateCellsState();
  auto upd_state = clock.restart().asMicroseconds();
  // todo Think about binary search with deleted
  if (ShouldCleanCells())
    CleanCells();
  auto clean_cells = clock.restart().asMicroseconds();
  if (ShouldCleanFood())
    CleanFood();
//  CleanFood();
  if (world_.cells_[0].lifetime_ == 500) {
    int t = 1;
  }
}

void Logic::ResolveCellCollisions(collisions::IdXPairs &colliding_cells_ids) const {
  for (auto &idx_pair : colliding_cells_ids) {
    auto &cell_1 = world_.cells_[idx_pair.first], &cell_2 = world_.cells_[idx_pair.second];
    collisions::CollisionResolver::ResolveCollisions(cell_1, cell_2);
  }
}

bool Logic::ShouldChangeDirection(core::Cell &cell) {
  return !cell.GetDirection()
      || (cell.HasTargetFood() && cell.lifetime_ % 15 == 0)
      || (!cell.HasTargetFood() && (cell.lifetime_ % cell.GetDirectionChangeFactor() == 0));
}

void Logic::ChooseDirections() {
  for (uint cell_idx = 0, food_idx = 0; auto &cell : world_.cells_) {
    if (cell.IsAlive()) {
      if (ShouldChangeDirection(cell)) {
        cell.ClearFoodTarget();
        if (cell.IsHunter()) {
          hunter_cell_logic_.ChooseDirection(cell, cell_idx, world_.cells_);
        } else if (cell.IsNonHunter()) {
          non_hunter_cell_logic_.ChooseDirection(cell, food_idx, world_.food_);
        } else {
          throw std::runtime_error("Invalid cell type");
        }
      }
    }
    ++cell_idx;
  }
}

void Logic::MoveCells() {
  for (auto &cell : world_.cells_) {
    if (cell.IsAlive()) {
//      cell.Move(cell.GetDirection().value());
      cell.Move2();
    }
    CheckCrossedBoundaries(cell);
  }
}

[[maybe_unused]] void Logic::TeleportCrossedBoundaries(core::Cell &cell) const {
  auto &pos = cell.GetPosition();
  if (pos.x < 0.0) {
    pos.x = world_.width_;
  } else if (pos.x > world_.width_) {
    pos.x = 0.0;
  }
  if (pos.y < 0.0) {
    pos.y = world_.height_;
  } else if (pos.y > world_.height_) {
    pos.y = 0.0;
  }
}

void Logic::CheckCrossedBoundaries(core::Cell &cell) const {
  auto &pos = cell.GetPosition();
  auto size = cell.GetSize();
  bool hit_the_wall = false;

  if (pos.x < 0.0 + size) {
    pos.x = size;
    hit_the_wall = true;
  } else if (pos.x > world_.width_ - size) {
    pos.x = world_.width_ - size;
    hit_the_wall = true;
  }

  if (pos.y < 0.0 + size) {
    pos.y = size;
    hit_the_wall = true;
  } else if (pos.y > world_.height_ - size) {
    pos.y = world_.height_ - size;
    hit_the_wall = true;
  }

  if (hit_the_wall) cell.ClearDirection();
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

// todo probably direction change factor is not working correctly yet
void Logic::UpdateCellsState() {
  std::vector<core::Cell> new_cells;
  for (auto &cell : world_.cells_) {
    if (cell.HasEnergyToDivide() && cell.DivisionCooldownPassed()) {
      new_cells.push_back(DivideCell(cell));
//      cell.ConsumeDivisionEnergy();
    } else if (cell.HasDecayed()) {
      core::Food food(core::Food(
          core::FoodType::K_FLORAL,
          cell.GetPosition(),
          cell.GetBaseNutritionValue()
      ));
      world_.AddFood(food);
      // todo
      cell.Delete();
      ++deleted_cells_num_;
    }
  }
  for (auto &cell : new_cells) {
    world_.AddCell(cell);
  }
}

core::Cell Logic::DivideCell(core::Cell &cell) {
  core::Cell new_cell(
      cell.type_,
      cell.GetPosition(),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  new_cell.AddEnergy(cell.energy_ / 2);
  new_cell.StartDivisionCooldown();
  new_cell.MoveX(cell.GetSize() * 2);
  cell.StartDivisionCooldown();
  cell.ConsumeDivisionEnergy();
  return new_cell;
}

bool CanKill(core::Cell &hunter_cell, core::Cell &prey_cell) {
  return prey_cell.GetSize() < hunter_cell.GetSize() * 1.5;
}

void Logic::HunterEat(collisions::IdXPairs &colliding_cell_ids) {
  std::vector<uint> eaten_cell_ids;
  for (auto &colliding_cell_id : colliding_cell_ids) {
    if (CanEat(colliding_cell_id)) {
      auto &prey_cell = ExtractPrey(colliding_cell_id);
      auto &hunter_cell = ExtractHunter(colliding_cell_id);
      if (std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), prey_cell.GetId()) != eaten_cell_ids.end())
        continue;
      if (!hunter_cell.IsHungry() || (!CanKill(hunter_cell, prey_cell) && !prey_cell.IsDead())) continue;
      if (prey_cell.HasShell()) {
        prey_cell.DamageShell(hunter_cell.GetPunchStrength());
        hunter_cell.ConsumePunchEnergy();
      } else {
        hunter_cell.AddEnergy(prey_cell.GetNutritionValue());
        hunter_cell.ClearFoodTarget();
        hunter_cell.ClearDirection();
        eaten_cell_ids.push_back(prey_cell.GetId());
        // todo
        prey_cell.Delete();
        ++deleted_cells_num_;
      }
    }
  }
  std::erase_if(
      colliding_cell_ids,
      [&eaten_cell_ids](collisions::IdxPair &pair) {
        return std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), pair.first) != eaten_cell_ids.end()
            || std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), pair.second) != eaten_cell_ids.end();
      }
  );
}

void Logic::NonHunterEat(collisions::FoodCellCollisions &colliding_food_cell_ids) {
  std::vector<uint> eaten_food_ids;
  for (auto &colliding_food_cell : colliding_food_cell_ids) {
    auto &cell = world_.cells_[colliding_food_cell.cell_idx];
    auto &food = world_.food_[colliding_food_cell.food_idx];
    if (std::find(eaten_food_ids.begin(), eaten_food_ids.end(), food.GetId()) != eaten_food_ids.end())
      continue;
    if (!cell.IsHungry() || !cell.IsAlive() || cell.IsHunter()) continue;
    cell.AddEnergy(food.GetNutritionValue());
    cell.ClearFoodTarget();
    cell.ClearDirection();
    eaten_food_ids.push_back(food.GetId());
    // todo
    food.Delete();
    ++deleted_food_num_;
  }
}

bool Logic::CanEat(collisions::IdxPair &cell_id_pair) {
  auto &first = world_.cells_[cell_id_pair.first], &second = world_.cells_[cell_id_pair.second];
  bool at_least_one_alive_hunter = (first.IsHunter() && !first.IsDead()) || (second.IsHunter() && !second.IsDead());

  if (!at_least_one_alive_hunter)
    return false;

  if (first.IsDead() || second.IsDead())
    return true;

  return (first.IsHunter() && second.IsNonHunter())
      || (first.IsNonHunter() && second.IsHunter());
}

core::Cell &Logic::ExtractHunter(collisions::IdxPair &cell_id_pair) {
  auto &first = world_.cells_[cell_id_pair.first], &second = world_.cells_[cell_id_pair.second];
  if (first.IsHunter() && !first.IsDead())
    return first;
  else
    return second;
}

core::Cell &Logic::ExtractPrey(collisions::IdxPair &cell_id_pair) {
  auto &first = world_.cells_[cell_id_pair.first], &second = world_.cells_[cell_id_pair.second];
  if (first.IsNonHunter() || first.IsDead())
    return first;
  else
    return second;
}

void Logic::Tick() {
  for (auto &cell : world_.cells_) {
    cell.Tick();
  }
  world_.Tick();
}

bool Logic::ShouldCleanCells() const {
  return true;
  return deleted_cells_num_ >= k_deleted_clear_threshold_;
}

bool Logic::ShouldCleanFood() const {
  return true;
  return deleted_food_num_ >= k_deleted_clear_threshold_;
}

void Logic::CleanCells() {
  std::erase_if(world_.cells_, [](core::Cell &cell) { return cell.IsDeleted(); });
  // todo it's for debug
//  std::erase_if(world_.cells_, [](core::Cell &cell) { return cell.IsDead(); });
  deleted_cells_num_ = 0;
}

void Logic::CleanFood() {
  std::erase_if(world_.food_, [](core::Food &food) { return food.IsDeleted(); });
  deleted_food_num_ = 0;
}
}
