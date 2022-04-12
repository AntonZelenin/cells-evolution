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
    cell->AddEnergy(cell->GetDivisionEnergy());
    cells.push_back(cell);
  }
  return cells;
}
}