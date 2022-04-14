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
  return genes_.at(genetics::GeneType::SIZE).value;
}

float Cell::GetSpeed() const {
  float speed = 0;
  if (HasTargetFood())
    speed = genes_.at(genetics::GeneType::HUNTING_SPEED).value * k_speed_size_coefficient_ * GetSize();
  else
    speed = genes_.at(genetics::GeneType::IDLE_SPEED).value * k_speed_size_coefficient_ * GetSize();
  if (HasShell())
    speed /= GetShell();
  return speed;
}

bool Cell::IsHunter() const {
  return type_ == core::CellType::K_HUNTER;
}

bool Cell::IsNonHunter() const {
  return type_ == core::CellType::K_NONHUNTER;
}

float Cell::GetMaxFoodDetectionDistance() const {
  return k_max_distance_food_detection_;
//  if (IsNonHunter())
//    return k_max_distance_food_detection_ * 0.9;
//  else
//    return k_max_distance_food_detection_ * 1.2;
}

std::optional<unsigned int> Cell::GetFoodTargetId() const {
  return food_target_id_;
}

void Cell::SetFoodTargetId(unsigned int food_id) {
  food_target_id_ = food_id;
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

unsigned int Cell::GetId() const {
  return id_;
}

void Cell::SetId(unsigned int id) {
  id_ = id;
}

void Cell::MoveX(float val) {
  position_.x += val;
}

void Cell::MoveY(float val) {
  position_.y += val;
}

// todo maybe update? do I need it?
void Cell::SetPosition(Position pos) {
  position_ = pos;
}

void Cell::Move(core::Vector2<float> const &direction) {
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

//bool Cell::CanConsume(float energy) const {
//  return energy_ + energy < GetMaxEnergy();
//}

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
  division_cooldown_ = static_cast<unsigned int>(genes_.at(genetics::GeneType::DIVISION_COOLDOWN).value);
}

int Cell::GetDirectionChangeFactor() const {
  return static_cast<int>(genes_.at(genetics::GeneType::DIRECTION_CHANGE_FACTOR).value);
}

bool Cell::HasShell() const {
  return GetShell() > 0.0;
}

bool Cell::HasTargetFood() const {
  return static_cast<bool>(food_target_id_);
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
  if (!IsDead()) {
    if (lifetime_ == std::numeric_limits<unsigned int>::max())
      lifetime_ = 0;
    else
      lifetime_++;
    if (division_cooldown_ > 0)
      division_cooldown_--;
    energy_ -= k_vital_functions_energy_consumption_ * GetSize();
    if (energy_ <= 0)
      shell_ /= 2.0;
  } else {
    if (time_to_decay_ > 0) time_to_decay_ -= 1;
  }
}

// todo every cell move to the same point when copy constructor is implemented
//Cell::Cell(const Cell &cell) {
//  position_ = Position(cell.position_.x, cell.position_.y);
//  energy_ = cell.energy_ / 2;
//  radius_ = cell.GetSize();
//  speed_ = cell.GetSpeed();
//  type_ = cell.type_;
//}
}
