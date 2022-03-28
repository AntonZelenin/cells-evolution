#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_

#include <vector>
#include "geometry.h"
#include "genes.h"
#include "base.h"

namespace cells_evo::core {
enum Type {
  K_HUNTER, K_NONHUNTER
};

class Cell : public core::Entity {
  Position position_;
 public:
  unsigned int id_;
  float size_;
  float speed_;
  Type type_;
  std::vector<Gene> genes{};

  Cell(float size, float speed, Type type, Position position);
  Position &GetPosition() override;
  void SetPosition(Position pos) override;
  unsigned int GetId() override;
  void SetId(unsigned int id);
};

class CellGenerator {
  static const int k_first_generation_size_ = 20;
  const float k_default_cell_size_ = 20.0;
  const float k_default_cell_speed_ = 1.0;
  const int k_min_distance_between_cells_ = 1;

  static Type GetCellType();
 public:
  [[nodiscard]] std::vector<Cell> Generate(int field_width, int field_height, int generation_size) const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
