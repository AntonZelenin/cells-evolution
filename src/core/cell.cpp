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

// todo maybe update? do I need it?
void Cell::SetPosition(Position pos) {
  position_ = pos;
}

void Cell::ConsumeEnergy() {
  energy_ -= size_ * speed_ * k_energy_consumption_coefficient_;
}

bool Cell::HasEnergy() const {
  return energy_ > 0;
}
void Cell::AddEnergy(float energy) {
  energy_ += energy;
}

std::vector<Cell> CellGenerator::Generate(
    int field_width,
    int field_height,
    int generation_size = CellGenerator::k_first_generation_size_
) const {
  std::vector<Cell> cells;
  cells.reserve(generation_size);
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width, field_height, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    cells.emplace_back(
        k_default_cell_size_,
        k_default_cell_speed_,
        k_default_cell_energy_,
        CellGenerator::GetCellType(),
        position
    );
  }
  return cells;
}

Type CellGenerator::GetCellType() {
  return Type::K_HUNTER;
}
}
