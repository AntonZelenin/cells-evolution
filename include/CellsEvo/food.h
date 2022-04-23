#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_H_

#include <vector>
#include "base.h"

namespace cells_evo::core {
enum FoodType {
  K_FLORAL,
  K_ANIMAL,
};

class Food : public EdibleEntity {
  Position position_;
  uint id_{};
  float radius_;
  float nutrition_value_;
  bool is_deleted_ = false;

  static float CalculateRadius(float nutrition_value);

 public:
  FoodType type_;

  explicit Food(FoodType type, Position position);
  Food(FoodType type, Position position, float nutrition_value);

  Position &GetPosition() override;
  [[nodiscard]] float GetSize() const override;
  [[nodiscard]] uint GetId() const override;
  [[nodiscard]] float GetNutritionValue() const override;
  [[nodiscard]] bool IsDeleted() const;
  void Delete();
  void SetId(uint id);
};

class FoodGenerator {
  const int k_min_distance_between_items_ = 1;

 public:
  [[nodiscard]] std::vector<Food> CreateFloralGeneration(
      int field_width,
      int field_height,
      int size
  ) const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_FOOD_H_
