#include "CellsEvo/cell_generator.h"

namespace cells_evo::core {
std::vector<std::shared_ptr<core::Cell>> CellGenerator::Generate(
    int field_width,
    int field_height,
    int generation_size,
    core::Type cell_type
) {
  std::vector<std::shared_ptr<core::Cell>> cells;
  cells.reserve(generation_size);
  auto positions = GenerateRandomPositions(
      field_width, field_height, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    auto cell = std::make_shared<Cell>(
        cell_type,
        position,
        genetic_engineer_.GenerateBaseGenes(cell_type)
    );
    cells.push_back(cell);
  }
  return cells;
}

// todo copy constructor?
std::shared_ptr<core::Cell> CellGenerator::DivideCell(core::Cell &cell) {
  auto new_cell = std::make_shared<core::Cell>(
      cell.type_,
      core::Position(cell.GetPosition().x, cell.GetPosition().y),
      genetic_engineer_.CopyGenes(cell.genes_)
  );
  new_cell->AddEnergy(cell.energy_ / 2);
  new_cell->StartDivisionCooldown();
  cell.StartDivisionCooldown();
  return new_cell;
}
}