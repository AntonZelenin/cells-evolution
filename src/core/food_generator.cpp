#include "CellsEvo/food_generator.h"
#include "CellsEvo/tools.h"

namespace cells_evo::core {
FoodGenerator::FoodGenerator(int world_width, int world_height) {
  world_height_ = world_height;
  world_width_ = world_width;
}

std::vector<Food> FoodGenerator::CreateFloralGeneration(int size) {
  std::vector<Food> foods;
  foods.reserve(size);
  auto positions = GenerateRandomPositions(
      world_width_, world_height_, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    Food food(FoodType::K_FLORAL, position);
    food.SetShape(drawer_.Get(food));
    foods.push_back(food);
  }
  return foods;
}
}