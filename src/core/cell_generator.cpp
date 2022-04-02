#include "CellsEvo/core/cell_generator.h"

namespace cells_evo::core {
std::vector<core::Cell> CellGenerator::Generate(
    int field_width,
    int field_height,
    int generation_size,
    core::Type cell_type
) {
  std::vector<core::Cell> cells;
  cells.reserve(generation_size);
  auto positions = GenerateRandomPositions(
      field_width, field_height, generation_size, k_min_distance_between_cells_
  );
  for (auto &position : positions) {
    cells.emplace_back(
        k_default_cell_size_,
        GetDefaultCellSpeed(cell_type),
        k_default_cell_energy_,
        cell_type,
        position,
        genetic_engineer_.GenerateBaseGenes(cell_type)
    );
  }
  return cells;
}

float CellGenerator::GetDefaultCellSpeed(core::Type cell_type) const {
  switch (cell_type) {
    case core::Type::K_HUNTER:return k_default_hunter_cell_speed_;
    case core::Type::K_NONHUNTER:return k_default_cell_speed_;
  }
}
}