#include "CellsEvo/cell.h"

namespace cells_evo::core {
Cell::Cell(
    Type type,
    Position position,
    std::vector<genetics::Gene> const &genes
)
    : position_(position) {
  id_ = 0;
  type_ = type;
  for (auto gene : genes) {
    genes_.insert({gene.type, gene});
  }
  energy_ = 0;
}

Position &Cell::GetPosition() {
  return position_;
}

float Cell::GetRadius() const {
  return genes_.at(genetics::GeneType::SIZE).value;
}

float Cell::GetSpeed() const {
  return genes_.at(genetics::GeneType::SPEED).value * k_speed_size_coefficient_ * GetRadius();
}

bool Cell::IsHunter() const {
  return type_ == core::Type::K_HUNTER;
}

bool Cell::IsNonHunter() const {
  return type_ == core::Type::K_NONHUNTER;
}

float Cell::GetMaxFoodDetectionDistance() const {
  if (IsNonHunter())
    return k_max_distance_food_detection_ * 0.9;
  else
    return k_max_distance_food_detection_ * 1.2;
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

unsigned int Cell::GetId() {
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

void Cell::ConsumeMovementEnergy() {
  energy_ -= GetRadius() * GetSpeed() * k_energy_consumption_coefficient_;
}

void Cell::ConsumeDivisionEnergy() {
  energy_ /= 2;
}

bool Cell::HasEnergy() const {
  return energy_ > 0;
}

void Cell::AddEnergy(float energy) {
  energy_ += energy;
  if (energy_ > GetMaxEnergy()) energy_ = GetRadius() * 2.f;
}

float Cell::GetMaxEnergy() const {
  return GetRadius() * 2.f;
}

float Cell::GetNutritionValue() const {
  return energy_ + GetBaseNutritionValue();
}

float Cell::GetBaseNutritionValue() const {
  return GetRadius() * 0.5f;
}

//bool Cell::CanConsume(float energy) const {
//  return energy_ + energy < GetMaxEnergy();
//}

bool Cell::IsHungry() const {
  return energy_ < GetMaxEnergy() * k_hunger_coefficient_;
}

float Cell::GetDivisionEnergy() const {
  return k_division_energy_size_coefficient_ * GetRadius();
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

void Cell::Tick() {
  if (lifetime_ == std::numeric_limits<unsigned int>::max())
    lifetime_ = 0;
  else
    lifetime_++;
  if (division_cooldown_ > 0)
    division_cooldown_--;
  energy_ -= k_vital_functions_energy_consumption_ * GetRadius();
}

// todo every cell move to the same point when copy constructor is implemented
//Cell::Cell(const Cell &cell) {
//  position_ = Position(cell.position_.x, cell.position_.y);
//  energy_ = cell.energy_ / 2;
//  size_ = cell.GetRadius();
//  speed_ = cell.GetSpeed();
//  type_ = cell.type_;
//}
}
