#include "CellsEvo/core/cell.h"

namespace cells_evo::core {
Cell::Cell(
    float energy,
    Type type,
    Position position,
    std::vector<genetics::Gene> const &genes
)
    : position_(position) {
  id_ = 0;
  type_ = type;
  energy_ = energy;
  for (auto gene : genes) {
    genes_.insert({gene.type, gene});
  }
}

Position &Cell::GetPosition() {
  return position_;
}

float Cell::GetRadius() const {
  return genes_.find(genetics::GeneType::SIZE)->second.value;
}

float Cell::GetSpeed() const {
  return genes_.find(genetics::GeneType::SPEED)->second.value * k_speed_size_coefficient_ * GetRadius();
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
  if (energy_ > GetRadius() * 2.f) energy_ = GetRadius() * 2.f;
}

bool Cell::HasEnergyToDivide() const {
  return energy_ > (k_division_energy_threshold_ * k_division_energy_size_coefficient_ * GetRadius());
}

bool Cell::DivisionCooldownPassed() const {
  return division_cooldown_ == 0;
}

void Cell::StartDivisionCooldown() {
  division_cooldown_ = static_cast<unsigned int>(genes_.find(genetics::GeneType::DIVISION_COOLDOWN)->second.value);
}

int Cell::GetDirectionChangeFactor() {
  return static_cast<int>(genes_.find(genetics::GeneType::DIRECTION_CHANGE_FACTOR)->second.value);
}

void Cell::Tick() {
  if (lifetime_ == std::numeric_limits<unsigned int>::max())
    lifetime_ = 0;
  else
    lifetime_++;
  if (division_cooldown_ > 0)
    division_cooldown_--;
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
