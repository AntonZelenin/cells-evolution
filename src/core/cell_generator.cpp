#include "CellsEvo/cell_generator.h"

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
  auto positions = GenerateRandomPositions(
      field_width_, field_height_, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    Cell cell(
        cell_type,
        position,
        genetic_engineer_.GenerateBaseGenes(cell_type)
    );
    cell.AddEnergy(cell.GetDivisionEnergy());
    cell.SetShapes(drawer_.GetAliveShape(cell), drawer_.GetDeadShape(cell));
    cells.push_back(cell);
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
}
