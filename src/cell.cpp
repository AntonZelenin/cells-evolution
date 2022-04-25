#include "CellsEvo/cell.h"

namespace cells_evo::core {
Cell::Cell(
    CellType type,
    Position position,
    std::vector<genetics::Gene> const &genes
)
    : position_(position) {
  id_ = 0;
  type_ = type;
  for (auto gene : genes) {
    genes_.insert({gene.type, gene});
    if (gene.type == genetics::GeneType::HARD_SHELL) shell_ = gene.value;
  }
  energy_ = 0;
}

Position &Cell::GetPosition() {
  return position_;
}

float Cell::GetSize() const {
  return genes_.at(genetics::GeneType::SIZE).value + GetShellThickness();
}

float Cell::GetShellThickness() const {
  return GetShell() * k_shell_thickness_coefficient_;
}

float Cell::GetSpeed() const {
  float speed;
  if (HasTargetFood())
    speed = GetHuntingSpeed();
  else
    speed = GetIdleSpeed();
  if (HasShell())
    speed /= GetShell();
  return speed;
}

float Cell::GetHuntingSpeed() const {
  return genes_.at(genetics::GeneType::HUNTING_SPEED).value * k_speed_size_coefficient_ * GetSize();
}

float Cell::GetIdleSpeed() const {
  return genes_.at(genetics::GeneType::IDLE_SPEED).value * k_speed_size_coefficient_ * GetSize();
}

bool Cell::IsHunter() const {
  return type_ == core::CellType::K_HUNTER;
}

bool Cell::IsNonHunter() const {
  return type_ == core::CellType::K_NONHUNTER;
}

float Cell::GetMaxFoodDetectionDistance() const {
  return k_max_distance_food_detection_ + GetSize() * 2;
//  if (IsNonHunter())
//    return k_max_distance_food_detection_ * 0.9;
//  else
//    return k_max_distance_food_detection_ * 1.2;
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

void Cell::ConsumeMovementEnergy(float speed) {
  auto energy = GetSize() * speed * k_energy_consumption_coefficient_;
//  if (HasShell())
//    energy += GetShell() * k_energy_consumption_coefficient_;
  energy_ -= energy;
}

void Cell::ConsumeDivisionEnergy() {
  energy_ /= 2;
}

bool Cell::IsDead() const {
  return energy_ <= 0;
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
  return GetSize() * 0.5f;
}

bool Cell::IsHungry() const {
  return energy_ < GetMaxEnergy() * k_hunger_coefficient_;
}

float Cell::GetPunchStrength() const {
  return GetSize() * k_punch_coefficient_;
}

void Cell::ConsumePunchEnergy() {
  energy_ -= GetSize() * k_energy_consumption_coefficient_;
}

float Cell::GetDivisionEnergy() const {
  return k_division_energy_size_coefficient_ * GetSize();
}

bool Cell::HasEnergyToDivide() const {
  return energy_ > GetDivisionEnergy();
}

bool Cell::DivisionCooldownPassed() const {
  return division_cooldown_ == 0;
}

void Cell::StartDivisionCooldown() {
  division_cooldown_ = static_cast<uint>(
      genes_.at(genetics::GeneType::DIVISION_COOLDOWN).value + 50.f * GetShell()
  );
}

int Cell::GetDirectionChangeFactor() const {
  return static_cast<int>(genes_.at(genetics::GeneType::DIRECTION_CHANGE_FACTOR).value);
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

void Cell::DamageShell(float value) {
  shell_ -= value;
}

float Cell::GetShell() const {
  if (shell_ < 1.0) return 0.0;
  return shell_;
}

bool Cell::HasDecayed() const {
  return IsDead() && time_to_decay_ == 0;
}

void Cell::Tick() {
  // todo maybe cells might die after movement, not here, and shell will not divide
  if (!IsDead()) {
    if (lifetime_ == std::numeric_limits<uint>::max())
      lifetime_ = 0;
    else
      ++lifetime_;
    if (division_cooldown_ > 0)
      --division_cooldown_;
    energy_ -= k_vital_functions_energy_consumption_ * GetSize();
    if (energy_ <= 0)
      shell_ /= 2.0;
  } else {
    if (time_to_decay_ > 0) time_to_decay_ -= 1;
  }
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
}
