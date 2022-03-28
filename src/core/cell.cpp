#include "CellsEvo/core.h"
#include "CellsEvo/core/cell.h"

namespace cells_evo::core {
Cell::Cell(float size, float speed, float energy, Type type, Position position) : position_(position) {
  id_ = 0;
  size_ = size;
  speed_ = speed;
  type_ = type;
  energy_ = energy;
}

Position &Cell::GetPosition() {
  return position_;
}

unsigned int Cell::GetId() {
  // todo duplicate
  return id_;
}

void Cell::SetId(unsigned int id) {
  // todo duplicate
  id_ = id;
}

void Cell::MoveX(float val) {
  position_.coordinates.x += val;
}

void Cell::MoveY(float val) {
  position_.coordinates.y += val;
}

// todo maybe update? do I need it?
void Cell::SetPosition(Position pos) {
  position_ = pos;
}

void Cell::ConsumeMovementEnergy() {
  energy_ -= size_ * speed_ * k_energy_consumption_coefficient_;
}

void Cell::ConsumeDivisionEnergy() {
  energy_ /= 2;
}

bool Cell::HasEnergy() const {
  return energy_ > 0;
}

void Cell::AddEnergy(float energy) {
  energy_ += energy;
}

bool Cell::HasEnergyToDivide() const {
  return energy_ > (k_division_energy_threshold_ * k_division_energy_size_coefficient_ * size_);
}

// todo every cell move to the same point when copy constructor is implemented
//Cell::Cell(const Cell &cell) {
//  position_ = Position(cell.position_.X(), cell.position_.Y());
//  energy_ = cell.energy_ / 2;
//  size_ = cell.size_;
//  speed_ = cell.speed_;
//  type_ = cell.type_;
//}

std::vector<Cell> CellGenerator::Generate(
    int field_width,
    int field_height,
    int generation_size,
    Type cell_type
) {
  std::vector<Cell> cells;
  cells.reserve(generation_size);
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width, field_height, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    cells.emplace_back(
        k_default_cell_size_,
        GetDefaultCellSpeed(cell_type),
        k_default_cell_energy_,
        cell_type,
        position
    );
  }
  return cells;
}

float CellGenerator::GetDefaultCellSpeed(Type cell_type) const {
  switch (cell_type) {
    case K_HUNTER:return k_default_hunter_cell_speed_;
    case K_NONHUNTER:return k_default_cell_speed_;
  }
}
}
