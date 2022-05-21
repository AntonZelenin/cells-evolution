#include "CellsEvo/core/cell_generator.h"

namespace cells_evo::core {
CellGenerator::CellGenerator(int field_width, int field_height) {
  field_width_ = field_width;
  field_height_ = field_height;
}

std::vector<Cell> CellGenerator::Generate(
    int generation_size,
    CellType cell_type
) {
  std::vector<Cell> cells;
  cells.reserve(generation_size);
  auto positions = random_positions_generator_.GenerateRandomPositions(
      field_width_, field_height_, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    cells.push_back(GenerateCell(position, cell_type));
  }
  return cells;
}

Cell CellGenerator::DivideCell(Cell &cell) {
  Cell new_cell(
      cell.type_,
      cell.GetPosition(),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  new_cell.AddEnergy(cell.GetEnergy() / 2);
  new_cell.StartDivisionCooldown();
  new_cell.MoveX(cell.GetSize() * 2);
  // todo duplicate
  new_cell.SetShapes(drawer_.GetAliveShape(cell), drawer_.GetDeadShape(cell));
  cell.StartDivisionCooldown();
  cell.ConsumeDivisionEnergy();
  return new_cell;
}

core::Cell CellGenerator::GenerateCell(Position position, core::CellType cell_type) {
  Cell cell(
      cell_type,
      position,
      genetic_engineer_.GenerateBaseGenes(cell_type)
  );
  cell.AddEnergy(cell.GetDivisionEnergy());
  cell.SetShapes(drawer_.GetAliveShape(cell), drawer_.GetDeadShape(cell));
  return cell;
}

std::vector<core::Cell> CircleCellGenerator::Generate(int generation_size, core::CellType cell_type) {
  return CellGenerator::Generate(generation_size, cell_type);
}
}
