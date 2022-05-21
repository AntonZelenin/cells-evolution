#include <utility>
#include <vector>
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/core/collision_resolution.h"
#include "SFML/System/Clock.hpp"

namespace cells_evo::logic {
Logic::Logic(
    core::World &world,
    float food_production_rate,
    core::CellGenerator cell_generator,
    core::FoodGenerator food_generator
)
    : world_(world), cell_generator_(std::move(cell_generator)), food_generator_(std::move(food_generator)) {
  food_production_rate_ = food_production_rate;
}

void Logic::WorldTick() {
  sf::Clock clock;
  Tick();
  ChooseDirections();
  auto dir = clock.restart().asMicroseconds();
  MoveCells();
  auto move = clock.restart().asMicroseconds();
  world_.SortFood();
  auto sort_food = clock.restart().asMicroseconds();
  world_.SortCells();
  auto sort_cells = clock.restart().asMicroseconds();
  auto colliding_cell_ids = collisions::CollisionDetector::DetectCellCollisions(world_.cells_);
  auto coll_cell = clock.restart().asMicroseconds();
  auto colliding_cell_food_ids = collisions::CollisionDetector::DetectCellFoodCollisions(world_.cells_, world_.food_);
  auto coll_food = clock.restart().asMicroseconds();
  ResolveHunterCollisions(colliding_cell_ids);
  auto hunter_eat = clock.restart().asMicroseconds();
  Eat(colliding_cell_food_ids);
  auto nonhunter_eat = clock.restart().asMicroseconds();
  ResolveCellCollisions(colliding_cell_ids);
  auto res_coll = clock.restart().asMicroseconds();
//  TeleportCrossedBoundaries();
  GenerateFood();
  auto generate_food = clock.restart().asMicroseconds();
  UpdateCellsState();
  auto upd_state = clock.restart().asMicroseconds();
  if (ShouldCleanCells())
    CleanCells();
  auto clean_cells = clock.restart().asMicroseconds();
  if (ShouldCleanFood())
    CleanFood();
  auto clean_food = clock.restart().asMicroseconds();
  if (world_.ticks_ == 400) {
    int t = 1;
  }
  CheckCellCrossedBoundaries();
  if (world_.ticks_ % 30 == 0)
    CheckFoodCrossedBoundaries();
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
    if (cell.IsAlive() && ShouldChangeDirection(cell)) {
      cell.ClearFoodTarget();
      if (cell.IsHunter()) {
        hunter_cell_logic_.ChooseDirection(cell, cell_idx, world_.cells_, food_idx, world_.food_);
      } else if (cell.IsNonHunter()) {
        non_hunter_cell_logic_.ChooseDirection(cell, food_idx, cell_idx, world_.food_, world_.cells_);
      } else {
        throw std::runtime_error("Invalid cell type");
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

void Logic::CheckCellCrossedBoundaries() const {
  for (auto &cell : world_.cells_) {
    if (!cell.IsDeleted() && ProcessCrossedWalls(cell.GetSize(), cell.GetPosition()))
      cell.ClearDirection();
  }
}

void Logic::CheckFoodCrossedBoundaries() const {
  for (auto &food : world_.food_) {
    if (!food.IsDeleted())
      ProcessCrossedWalls(food.GetSize(), food.GetPosition());
  }
}

bool Logic::ProcessCrossedWalls(float size, core::Position &pos) const {
  bool hit_wall = false;
  if (pos.x < 0.0 + size) {
    pos.x = size;
    hit_wall = true;
  } else if (pos.x > world_.width_ - size) {
    pos.x = world_.width_ - size;
    hit_wall = true;
  }

  if (pos.y < 0.0 + size) {
    pos.y = size;
    hit_wall = true;
  } else if (pos.y > world_.height_ - size) {
    pos.y = world_.height_ - size;
    hit_wall = true;
  }
  return hit_wall;
}

void Logic::GenerateFood(int number) {
  if (ShouldGenerateFood()) {
    world_.AddFood(std::move(
        food_generator_.CreateFloralGeneration(number)
    ));
  }
  for (auto &food_deposit : world_.food_deposits_) {
    if (ShouldGenerateFood()) {
      world_.AddFood(std::move(
          food_generator_.GenerateFromDeposit(food_deposit)
      ));
    }
  }
}

void Logic::ProduceFirstFoodGeneration(int number) {
  world_.AddFood(std::move(
      food_generator_.CreateFloralGeneration(number)
  ));
}

bool Logic::ShouldGenerateFood() {
  // generates food only once in N secs on average
  return random_generator_.GetUniformReal(0, food_production_rate_) <= 1;
}

// todo probably direction change factor is not working correctly yet
void Logic::UpdateCellsState() {
  std::vector<core::Cell> new_cells;
  for (auto &cell : world_.cells_) {
    // todo this is bad that I need to remember about deleted cells everywhere
    if (cell.IsDeleted()) continue;
    if (cell.HasEnergyToDivide() && cell.DivisionCooldownPassed()) {
      new_cells.push_back(cell_generator_.DivideCell(cell));
    } else if (cell.HasDecayed()) {
      GenerateFloralFood(cell);
    } else if (!cell.IsHolistic()) {
      GenerateAnimalFood(cell);
    }
  }
  for (auto &cell : new_cells) {
    world_.AddCell(cell);
  }
}

void Logic::GenerateFloralFood(core::Cell &cell) {
  GenerateFoodFromCell(core::FoodType::K_FLORAL, cell);
}

void Logic::GenerateAnimalFood(core::Cell &cell) {
  GenerateFoodFromCell(core::FoodType::K_ANIMAL, cell);
}

void Logic::GenerateFoodFromCell(core::FoodType food_type, core::Cell &cell) {
  core::Food food(core::Food(
      food_type,
      cell.GetPosition(),
      cell.GetBaseNutritionValue()
  ));
  food.SetShape(food_drawer_.Get(food));
  world_.AddFood(food);
  cell.Delete();
  ++deleted_cells_num_;
}

void Logic::ResolveHunterCollisions(collisions::IdXPairs &colliding_cell_ids) {
  std::vector<uint> eaten_cell_ids;
  for (auto &colliding_cell_id : colliding_cell_ids) {
    if (CanEat(colliding_cell_id)) {
      auto &prey_cell = ExtractPrey(colliding_cell_id);
      auto &hunter_cell = ExtractHunter(colliding_cell_id);
      if (std::find(eaten_cell_ids.begin(), eaten_cell_ids.end(), prey_cell.GetId()) != eaten_cell_ids.end())
        continue;
      if (prey_cell.IsHolistic()) {
        prey_cell.InflictDamage(hunter_cell.GetPunchStrength());
        hunter_cell.ConsumePunchEnergy();
      } else if (hunter_cell.IsHungry()) {
        hunter_cell.AddEnergy(prey_cell.GetNutritionValue());
        hunter_cell.ClearFoodTarget();
        hunter_cell.ClearDirection();
        eaten_cell_ids.push_back(prey_cell.GetId());
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

void Logic::Eat(collisions::FoodCellCollisions &colliding_food_cell_ids) {
  std::vector<uint> eaten_food_ids;
  for (auto &colliding_food_cell : colliding_food_cell_ids) {
    auto &cell = world_.cells_[colliding_food_cell.cell_idx];
    auto &food = world_.food_[colliding_food_cell.food_idx];

    if (food.IsDeleted()) continue;
    if (std::find(eaten_food_ids.begin(), eaten_food_ids.end(), food.GetId()) != eaten_food_ids.end())
      continue;
    if (!cell.IsHungry() || !cell.IsAlive()) continue;

    if ((cell.IsHunter() && food.IsAnimal()) || (cell.IsNonHunter() && food.IsFloral())) {
      cell.AddEnergy(food.GetNutritionValue());
      cell.ClearFoodTarget();
      cell.ClearDirection();
      eaten_food_ids.push_back(food.GetId());
      food.Delete();
      ++deleted_food_num_;
    }
  }
}

bool Logic::CanEat(collisions::IdxPair &cell_id_pair) {
  auto &first = world_.cells_[cell_id_pair.first], &second = world_.cells_[cell_id_pair.second];

  if (first.IsDeleted() || second.IsDeleted()) return false;
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
    if (!cell.IsDeleted()) {
      CellTick(cell);
    }
  }
  world_.Tick();
}

void Logic::CellTick(core::Cell &cell) {
  // todo maybe cells might die after movement, not here, and shell will not divide
  if (!cell.IsDead()) {
    ++cell.lifetime_;
    if (cell.division_cooldown_ > 0)
      --cell.division_cooldown_;
    cell.ConsumeVitalFunctionsEnergy();
  } else if (cell.time_to_decay_ > 0) {
    cell.time_to_decay_ -= 1;
  }
}

bool Logic::ShouldCleanCells() const {
  return deleted_cells_num_ >= k_deleted_clear_threshold_;
}

bool Logic::ShouldCleanFood() const {
  return deleted_food_num_ >= k_deleted_clear_threshold_;
}

void Logic::CleanCells() {
  std::erase_if(world_.cells_, [](core::Cell &cell) { return cell.IsDeleted(); });
  deleted_cells_num_ = 0;
}

void Logic::CleanFood() {
  std::erase_if(world_.food_, [](core::Food &food) { return food.IsDeleted(); });
  deleted_food_num_ = 0;
}

void Logic::GenerateNonhunterCells(int number) {
  world_.AddCells(std::move(
      cell_generator_.Generate(
          number,
          core::CellType::K_NONHUNTER
      )
  ));
}

void Logic::GenerateHunterCells(int number) {
  world_.AddCells(std::move(
      cell_generator_.Generate(
          number,
          core::CellType::K_HUNTER
      ))
  );
}
}
