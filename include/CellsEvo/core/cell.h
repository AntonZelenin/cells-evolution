#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_

#include <vector>
#include <unordered_map>
#include "geometry.h"
#include "genetics/genes.h"
#include "base.h"

namespace cells_evo::core {
enum Type {
  K_HUNTER,
  K_NONHUNTER,
};

class Cell : public Entity {
  const float k_energy_consumption_coefficient_ = 0.0005;
  const float k_division_energy_threshold_ = 6;
  const float k_division_energy_size_coefficient_ = 0.2;
  const float k_nutrition_value_coefficient_ = 0.2;
  const float k_speed_size_coefficient_ = 0.1;

  std::optional<unsigned int> food_target_id_{};
  std::optional<core::Vector2<float>> direction_{};

  Position position_;

 public:
  unsigned int lifetime_ = 0;
  constexpr static const float k_max_distance_food_detection_ = 180;

  // todo everything is public =*(
  // this is the energy the cell will give if eaten
  float energy_{};
  unsigned int id_{};
  Type type_;
  std::unordered_map<genetics::GeneType, genetics::Gene> genes_{};

  Cell(
      float energy,
      Type type,
      Position position,
      std::vector<genetics::Gene> const &genes
  );
//  Cell(const Cell &cell);

  Position &GetPosition() override;
  [[nodiscard]] float GetSize() const override;
  [[nodiscard]] float GetSpeed() const;

  [[nodiscard]] std::optional<unsigned int> GetFoodTargetId() const;
  void SetFoodTargetId(unsigned int food_id);
  void ClearFoodTarget() { food_target_id_ = {}; }

  [[nodiscard]] std::optional<core::Vector2<float>> GetDirection() const;
  void SetDirection(core::Vector2<float> food_id);
//  void ClearFoodTarget() { food_target_id_ = {}; }

  void SetPosition(Position pos) override;
  unsigned int GetId() override;
  void SetId(unsigned int id);
  void MoveX(float val);
  void MoveY(float val);
  void ConsumeMovementEnergy();
  void ConsumeDivisionEnergy();
  void AddEnergy(float energy);
  [[nodiscard]] bool HasEnergy() const;
  [[nodiscard]] bool HasEnergyToDivide() const;

  int GetDirectionChangeFactor();
  [[nodiscard]] inline float GetNutritionValue() const { return energy_ /* + GetSize() * k_nutrition_value_coefficient_*/; };
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
