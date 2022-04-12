#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_

#include <vector>
#include "base.h"

namespace cells_evo::core {
class Food : public EdibleEntity {
  Position position_;
 public:
  unsigned int id_;

  explicit Food(unsigned int id, Position position);
  Position &GetPosition() override;
  [[nodiscard]] float GetRadius() const override;
  void SetPosition(Position) override;
  unsigned int GetId() override;
  void SetId(unsigned int id);
  [[nodiscard]] inline float GetNutritionValue() const override { return 5.0; };
};

class FoodGenerator {
  IndexDriver index_driver_{};
  const int k_min_distance_between_items_ = 1;

 public:
  [[nodiscard]] std::vector<std::shared_ptr<Food>> Generate(int field_width, int field_height, int size);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
