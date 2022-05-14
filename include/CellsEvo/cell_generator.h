#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_GENERATOR_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_GENERATOR_H_

#include "CellsEvo/genetics/engineer.h"
#include "CellsEvo/graphics/drawer.h"

namespace cells_evo::core {
class CellGenerator {
  int field_width_;
  int field_height_;
  const int k_min_distance_between_cells_ = 10;
  graphics::CellDrawer drawer_{};

  // todo two classes have genetics engineers, that's bad
  genetics::Engineer genetic_engineer_{};

 public:
  CellGenerator(int field_width, int field_height);

  [[nodiscard]] std::vector<core::Cell> Generate(
      int generation_size,
      core::CellType cell_type
  );
  core::Cell DivideCell(core::Cell &cell);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_GENERATOR_H_
