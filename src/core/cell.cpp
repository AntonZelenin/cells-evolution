#include "CellsEvo/core.h"
#include "CellsEvo/core/cell.h"

namespace cells_evo::core {
Cell::Cell(float size, float speed, Type type, Position position) : position_(position) {
  this->id_ = 0;
  this->size_ = size;
  this->speed_ = speed;
  this->type_ = type;
}

Position &Cell::GetPosition() {
  return this->position_;
}

unsigned int Cell::GetId() {
  // todo duplicate
  return this->id_;
}

void Cell::SetId(unsigned int id) {
  // todo duplicate
  this->id_ = id;
}

// todo maybe update? do I need it?
void Cell::SetPosition(Position pos) {
  this->position_ = pos;
}

std::vector<Cell> CellGenerator::Generate(
    int field_width,
    int field_height,
    int generation_size = CellGenerator::k_first_generation_size_
) const {
  std::vector<Cell> cells;
  cells.reserve(generation_size);
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width, field_height, generation_size, this->k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    cells.emplace_back(
        this->k_default_cell_size_,
        this->k_default_cell_speed_,
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
