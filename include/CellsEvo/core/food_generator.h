#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_GENERATOR_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_GENERATOR_H_

#include <vector>
#include "CellsEvo/graphics/drawer.h"
#include "tools.h"

namespace cells_evo::core {
class FoodGenerator {
  const int k_min_distance_between_items_ = 1;

  int world_width_;
  int world_height_;

  graphics::FoodDrawer drawer_{};
  RandomPositionsGenerator random_positions_generator_{};

  Food Generate(Position &position);
 public:
  FoodGenerator(int world_width, int world_height);
  [[nodiscard]] std::vector<Food> CreateFloralGeneration(int size);
  [[nodiscard]] Food GenerateFromDeposit(FoodDeposit food_deposit);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_GENERATOR_H_
