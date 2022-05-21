#include "CellsEvo/core/food_generator.h"
#include "CellsEvo/core/tools.h"

namespace cells_evo::core {
FoodGenerator::FoodGenerator(int world_width, int world_height) {
  world_height_ = world_height;
  world_width_ = world_width;
}

std::vector<Food> FoodGenerator::CreateFloralGeneration(int size) {
  std::vector<Food> foods;
  foods.reserve(size);
  auto positions = random_positions_generator_.GenerateRandomPositions(
      world_width_, world_height_, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    foods.push_back(Generate(position));
  }
  return foods;
}

Food FoodGenerator::GenerateFromDeposit(FoodDeposit food_deposit) {
  return Generate(
      random_positions_generator_.GenerateRandomPositionsInRadius(
          food_deposit.center_,
          food_deposit.radius_,
          world_width_,
          world_height_,
          1
      )[0]
  );
}

Food FoodGenerator::Generate(Position &position) {
  Food food(FoodType::K_FLORAL, position);
  food.SetShape(drawer_.Get(food));
  return food;
}
}
