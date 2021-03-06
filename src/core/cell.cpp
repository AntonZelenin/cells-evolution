#include <utility>

#include "CellsEvo/core/cell.h"

namespace cells_evo::core {
Cell::Cell(
    CellType type,
    Position position,
    std::vector<genetics::Gene> const &genes
)
    : position_(position) {
  for (auto gene : genes) {
    genes_.insert({gene.type, gene});
    if (gene.type == genetics::GeneType::HARD_SHELL) shell_ = gene.value;
  }

  type_ = type;
  size_ = genes_.at(genetics::GeneType::SIZE).value;
  integrity_ = size_;
  hunting_speed_ = genes_.at(genetics::GeneType::HUNTING_SPEED).value * k_speed_size_coefficient_ * GetRawSize();
  idle_speed_ = genes_.at(genetics::GeneType::IDLE_SPEED).value * k_speed_size_coefficient_ * GetRawSize();
  direction_change_factor_ = static_cast<int>(genes_.at(genetics::GeneType::DIRECTION_CHANGE_FACTOR).value);
  base_division_cooldown_ = static_cast<uint>(
      genes_.at(genetics::GeneType::DIVISION_COOLDOWN).value + 50.f * GetShell()
  );

  if (IsHunter()) {
    base_division_cooldown_ *= 2.f;
  } else {
    ignores_hunter_near_food_ = genes_.at(genetics::GeneType::IGNORE_HUNTER_NEAR_FOOD).value > 0.5;
    ignores_food_near_hunter_ = genes_.at(genetics::GeneType::IGNORE_FOOD_NEAR_HUNTER).value > 0.5;
    if (ignores_hunter_near_food_ && ignores_food_near_hunter_) {
      if (genes_.at(genetics::GeneType::IGNORE_HUNTER_IS_RECESSIVE).value > 0.5) {
        ignores_hunter_near_food_ = false;
      } else {
        ignores_food_near_hunter_ = false;
      }
    }
  }
}

Position &Cell::GetPosition() {
  return position_;
}

float Cell::GetSize() const {
  return size_ + GetShellThickness();
}

float Cell::GetRawSize() const {
  return size_;
}

float Cell::GetShellThickness() const {
  return GetShell() * k_shell_thickness_coefficient_;
}

float Cell::GetSpeed() const {
  float speed;
  if (HasTargetFood() || is_running_)
    speed = GetHuntingSpeed();
  else
    speed = GetIdleSpeed();
  if (HasShell())
    speed /= GetShell() / 10.f;
  return speed;
}

float Cell::GetHuntingSpeed() const {
  return hunting_speed_;
}

float Cell::GetIdleSpeed() const {
  return idle_speed_;
}

bool Cell::IsHunter() const {
  return type_ == core::CellType::K_HUNTER;
}

bool Cell::IsNonHunter() const {
  return type_ == core::CellType::K_NONHUNTER;
}

float Cell::GetMaxFoodDetectionDistance() const {
  return k_max_distance_food_detection_ + GetSize() * 2;
}

[[nodiscard]] std::optional<core::Vector2<float>> Cell::GetDirection() const {
  return direction_;
}

void Cell::SetDirection(core::Vector2<float> direction) {
  direction_ = direction;
}

void Cell::ClearDirection() {
  direction_ = {};
}

uint Cell::GetId() const {
  return id_;
}

void Cell::SetId(uint id) {
  id_ = id;
}

void Cell::MoveX(float val) {
  position_.x += val;
}

void Cell::Move(core::Vector2<float> const &direction) {
  // todo they don't move after division
  auto speed = GetSpeed();
  GetPosition() += direction * speed;
  ConsumeMovementEnergy(speed);
}

void Cell::Move2() {
  // todo they don't end_speed after division
  auto speed = GetSpeed();
  GetPosition() += direction_.value() * speed;
  ConsumeMovementEnergy(speed);
}

void Cell::ConsumeMovementEnergy(float speed) {
  auto energy = GetSize() * speed * k_energy_consumption_coefficient_;
  if (HasShell())
    energy += GetShell() * k_energy_consumption_coefficient_;
  ConsumeEnergy(energy);
}

void Cell::ConsumeDivisionEnergy() {
  energy_ /= 2;
}

void Cell::ConsumeVitalFunctionsEnergy() {
  ConsumeEnergy(k_vital_functions_energy_consumption_ * GetSize());
}

bool Cell::IsDead() const {
  return !is_alive_;
}

bool Cell::IsAlive() const {
  return !IsDeleted() && !IsDead();
}

void Cell::AddEnergy(float energy) {
  energy_ += energy;
  energy_ = std::min(energy_, GetMaxEnergy());
}

float Cell::GetMaxEnergy() const {
  return GetSize() * 3.f;
}

float Cell::GetNutritionValue() const {
  return energy_ + GetBaseNutritionValue();
}

float Cell::GetBaseNutritionValue() const {
  return GetSize() * k_base_nutrition_value_coefficient_;
}

bool Cell::IsHungry() const {
  return energy_ < GetMaxEnergy() * k_hunger_coefficient_;
}

float Cell::GetPunchStrength() const {
  return GetSize() * k_punch_coefficient_;
}

void Cell::ConsumePunchEnergy() {
  ConsumeEnergy(GetSize() * k_energy_consumption_coefficient_);
}

float Cell::GetDivisionEnergy() const {
  if (HasShell())
    return GetSize() * 1.4f;
  return GetSize() * 0.9f;
}

bool Cell::HasEnergyToDivide() const {
  return energy_ > GetDivisionEnergy();
}

bool Cell::DivisionCooldownPassed() const {
  return division_cooldown_ == 0;
}

void Cell::StartDivisionCooldown() {
  division_cooldown_ = base_division_cooldown_;
}

float Cell::GetEnergy() const {
  return energy_;
}

int Cell::GetDirectionChangeFactor() const {
  return direction_change_factor_;
}

bool Cell::HasShell() const {
  return GetShell() > 0.0;
}

void Cell::SetHasFoodTarget() {
  has_target_food_ = true;
}

bool Cell::HasTargetFood() const {
  return has_target_food_;
}

void Cell::InflictDamage(float value) {
  if (HasShell())
    shell_ -= value;
  else
    integrity_ -= value;
  if (!IsHolistic())
    Kill();
}

bool Cell::IsHolistic() const {
  return integrity_ > 0.f;
}

float Cell::GetShell() const {
  if (shell_ < 10.0) return 0.0;
  return shell_;
}

bool Cell::HasDecayed() const {
  return IsDead() && time_to_decay_ == 0;
}

void Cell::ClearFoodTarget() {
  has_target_food_ = false;
}

bool Cell::IsDeleted() const {
  return is_deleted_;
}

void Cell::Delete() {
  is_deleted_ = true;
}

sf::CircleShape &Cell::GetShape() {
  return shape_;
}

void Cell::ConsumeEnergy(float energy) {
  energy_ -= energy;
  if (energy_ <= 0) {
    Kill();
  }
}

void Cell::SetShapes(sf::CircleShape alive_shape, sf::CircleShape dead_shape) {
  if (IsAlive()) {
    shape_ = std::move(alive_shape);
    dead_shape_ = std::move(dead_shape);
  } else {
    shape_ = std::move(dead_shape);
  }
}

void Cell::SetIsRunning(bool is_running) {
  is_running_ = is_running;
}

bool Cell::IgnoresHunterNearFood() const {
  return ignores_hunter_near_food_;
}

bool Cell::IgnoresFoodNearHunter() const {
  return ignores_food_near_hunter_;
}

void Cell::Kill() {
  is_alive_ = false;
  shell_ /= 2.0;
  integrity_ /= 2.0;
  shape_ = dead_shape_;
}

float Cell::GetClanValue() const {
  return genes_.at(genetics::GeneType::CLAN).value;
}
}
