#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_

namespace cells_evo::core {
class Food : Entity {
  Position position_;
 public:
  unsigned int id_;
  constexpr static const float k_default_energy_value_ = 3;

  explicit Food(Position position);

  Position &GetPosition() override;

  void SetPosition(Position) override;

  unsigned int GetId() override;

  void SetId(unsigned int id);
};

class FoodGenerator {
  static const int k_first_generation_size_ = 20;
  const int k_min_distance_between_items_ = 1;

 public:
  [[nodiscard]] std::vector<Food> Generate(int field_width, int field_height, int size) const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_FOOD_H_
