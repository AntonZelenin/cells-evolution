#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_

#include <vector>
#include <unordered_map>
#include "geometry.h"
#include "CellsEvo/genetics/genes.h"
#include "base.h"

namespace cells_evo::core {
enum CellType {
  K_HUNTER,
  K_NONHUNTER,
};

class Cell : public EdibleEntity {
  const float k_energy_consumption_coefficient_ = 0.0005;
  const float k_division_energy_size_coefficient_ = 1.0;
  const float k_hunger_coefficient_ = 0.7;
  const float k_punch_coefficient_ = 0.01;
  const float k_speed_size_coefficient_ = 0.27;
  const float k_vital_functions_energy_consumption_ = 0.00005;
  constexpr static const float k_max_distance_food_detection_ = 180;

  std::optional<unsigned int> food_target_id_{};
  std::optional<core::Vector2<float>> direction_{};
  float shell_{};

  Position position_;

 public:
  unsigned int lifetime_ = 0;
  unsigned int division_cooldown_ = 0;

  // todo everything is public =*(
  float energy_{};
  unsigned int id_{};
  CellType type_;
  std::unordered_map<genetics::GeneType, genetics::Gene> genes_{};

  Cell(
      CellType type,
      Position position,
      std::vector<genetics::Gene> const &genes
  );
//  Cell(const Cell &cell);

  void AddEnergy(float energy);
  void ClearDirection();
  void ClearFoodTarget() { food_target_id_ = {}; }
  void ConsumeMovementEnergy();
  void ConsumeDivisionEnergy();
  void DamageShell(float value);
  void SetDirection(core::Vector2<float> food_id);
  void SetFoodTargetId(unsigned int food_id);
  void SetId(unsigned int id);
  void SetPosition(Position pos) override;
  void MoveX(float val);
  void MoveY(float val);
  void StartDivisionCooldown();
  void Tick();

  unsigned int GetId() override;
  Position &GetPosition() override;
  [[nodiscard]] float GetSize() const override;
  [[nodiscard]] float GetMaxFoodDetectionDistance() const;
  [[nodiscard]] float GetSpeed() const;
  [[nodiscard]] std::optional<unsigned int> GetFoodTargetId() const;
  [[nodiscard]] int GetDirectionChangeFactor() const;
  [[nodiscard]] float GetMaxEnergy() const;
  [[nodiscard]] float GetShell() const;
  [[nodiscard]] float GetNutritionValue() const override;
  [[nodiscard]] float GetBaseNutritionValue() const;
  [[nodiscard]] bool IsDead() const;
  [[nodiscard]] bool HasEnergyToDivide() const;
  [[nodiscard]] bool DivisionCooldownPassed() const;
  [[nodiscard]] std::optional<core::Vector2<float>> GetDirection() const;
//  [[nodiscard]] bool CanConsume(float energy) const;
  [[nodiscard]] bool IsHungry() const;
  [[nodiscard]] float GetDivisionEnergy() const;
  [[nodiscard]] bool IsHunter() const;
  [[nodiscard]] bool IsNonHunter() const;
  [[nodiscard]] bool HasShell() const;
  [[nodiscard]] float GetPunchStrength() const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
