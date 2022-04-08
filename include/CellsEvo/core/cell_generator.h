#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_GENERATOR_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_GENERATOR_H_

#include "genetics/engineer.h"

namespace cells_evo::core {
class CellGenerator {
  const float k_default_hunter_cell_speed_ = 1.4;
  const float k_default_cell_energy_ = 10.0;
  const int k_min_distance_between_cells_ = 1;
  // todo two classes have genetics engineers, that's bad
  genetics::Engineer genetic_engineer_{};

 public:
  [[nodiscard]] std::vector<std::shared_ptr<core::Cell>> Generate(
      int field_width,
      int field_height,
      int generation_size,
      core::Type cell_type
  );
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_GENERATOR_H_
