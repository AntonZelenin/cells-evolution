#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_H_

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
  const float k_energy_consumption_coefficient_ = 0.0012;
  const float k_division_energy_size_coefficient_ = 1.0;
  const float k_hunger_coefficient_ = 0.7;
  const float k_punch_coefficient_ = 0.0012;
  const float k_speed_size_coefficient_ = 0.027;
  const float k_vital_functions_energy_consumption_ = 0.00015;
  const float k_max_distance_food_detection_ = 180;
  const float k_shell_thickness_coefficient_ = 0.1;

  std::optional<unsigned int> food_target_id_{};
  std::optional<core::Vector2<float>> direction_{};
  float shell_{};
  // 60 seconds until dead cell turns into food * 60 fps
  unsigned int time_to_decay_ = 60 * 60;
  Position position_;

  void ConsumeMovementEnergy(float speed);

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

  void AddEnergy(float energy);
  void ClearDirection();
  void ClearFoodTarget() { food_target_id_ = {}; }
  void ConsumeDivisionEnergy();
  void DamageShell(float value);
  void SetDirection(core::Vector2<float> food_id);
  void SetFoodTargetId(unsigned int food_id);
  void SetId(unsigned int id);
  void SetPosition(Position pos) override;
  void MoveX(float val);
  void StartDivisionCooldown();
  void Tick();
  void ConsumePunchEnergy();
  void Move(Vector2<float> const &direction);

  Position &GetPosition() override;
  [[nodiscard]] unsigned int GetId() const override;
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
  [[nodiscard]] bool IsHungry() const;
  [[nodiscard]] float GetDivisionEnergy() const;
  [[nodiscard]] bool IsHunter() const;
  [[nodiscard]] bool IsNonHunter() const;
  [[nodiscard]] bool HasShell() const;
  [[nodiscard]] float GetPunchStrength() const;
  [[nodiscard]] bool HasTargetFood() const;
  [[nodiscard]] bool HasDecayed() const;
  [[nodiscard]] float GetShellThickness() const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CELL_H_
