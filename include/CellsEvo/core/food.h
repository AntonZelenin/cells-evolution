#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_

#include "base.h"
#include "SFML/Graphics/RectangleShape.hpp"

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
  sf::RectangleShape shape_;

  static float CalculateRadius(float nutrition_value);

 public:
  FoodType type_;

  Food(FoodType type, Position position);
  Food(FoodType type, Position position, float nutrition_value);

  Position &GetPosition() override;
  [[nodiscard]] float GetSize() const override;
  [[nodiscard]] uint GetId() const override;
  [[nodiscard]] float GetNutritionValue() const override;
  [[nodiscard]] bool IsDeleted() const;
  void Delete();
  void SetId(uint id);
  void SetShape(sf::RectangleShape shape);
  sf::RectangleShape &GetShape();
};

class FoodDeposit {
 public:
  Position center_;
  float radius_;

  FoodDeposit(Position center, float radius);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
