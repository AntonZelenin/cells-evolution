#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_GENERATOR_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_GENERATOR_H_

#include <vector>
#include "CellsEvo/graphics/drawer.h"

namespace cells_evo::core {
class FoodGenerator {
  const int k_min_distance_between_items_ = 1;
  int world_width_;
  int world_height_;
  graphics::FoodDrawer drawer_{};

 public:
  FoodGenerator(int world_width, int world_height);
  [[nodiscard]] std::vector<Food> CreateFloralGeneration(int size);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_GENERATOR_H_
