#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_

#include <vector>
#include <unordered_map>
#include "geometry.h"
#include "CellsEvo/genetics/genes.h"
#include "base.h"
#include "SFML/Graphics/CircleShape.hpp"

namespace cells_evo::core {
enum CellType {
  K_HUNTER,
  K_NONHUNTER,
};

class Cell : public EdibleEntity {
  constexpr static const float k_base_nutrition_value_coeff_ = 0.5;
  constexpr static const float k_energy_consumption_coefficient_ = 0.0005;
  constexpr static const float k_vital_functions_energy_consumption_ = 0.0002;
  constexpr static const float k_hunger_coefficient_ = 0.6;
  constexpr static const float k_punch_coefficient_ = 0.0008;
  constexpr static const float k_speed_size_coefficient_ = 0.07;
  constexpr static const float k_max_distance_food_detection_ = 200;
  constexpr static const float k_shell_thickness_coefficient_ = 0.1;

  bool is_deleted_ = false;
  bool is_alive_ = true;
  bool has_target_food_ = false;
  bool is_running_ = false;
  bool ignores_hunter_near_food_;
  bool ignores_food_near_hunter_;
  std::optional<core::Vector2<float>> direction_{};
  // 60 seconds until dead cell turns into food * 60 fps
  Position position_;
  float energy_ = 0;
  float size_;
  float hunting_speed_;
  float idle_speed_;
  int direction_change_factor_;
  uint base_division_cooldown_;
  sf::CircleShape shape_;
  sf::CircleShape dead_shape_;

  void ConsumeMovementEnergy(float speed);
  void ConsumeEnergy(float energy);

  [[nodiscard]] float GetRawSize() const;
  [[nodiscard]] float GetMaxEnergy() const;

 public:
  uint time_to_decay_ = 60 * 60;
  float shell_ = 0;
  uint lifetime_ = 0;
  uint division_cooldown_ = 0;

  // todo everything is public =*(
  uint id_ = 0;
  CellType type_;
  std::unordered_map<genetics::GeneType, genetics::Gene> genes_{};

  Cell(
      CellType type,
      Position position,
      std::vector<genetics::Gene> const &genes
  );

  void AddEnergy(float energy);
  void ClearDirection();
  void ClearFoodTarget();
  void ConsumeDivisionEnergy();
  void DamageShell(float value);
  void SetDirection(core::Vector2<float> food_id);
  void SetHasFoodTarget();
  void SetId(uint id);
  void SetIsRunning(bool is_running);
  void SetShapes(sf::CircleShape alive_shape, sf::CircleShape dead_shape);
  void MoveX(float val);
  void StartDivisionCooldown();
  void ConsumePunchEnergy();
  void Move(Vector2<float> const &direction);
  void Move2();
  void Delete();
  void ConsumeVitalFunctionsEnergy();
  void Kill();

  Position &GetPosition() override;
  [[nodiscard]] uint GetId() const override;
  [[nodiscard]] float GetSize() const override;
  [[nodiscard]] float GetMaxFoodDetectionDistance() const;
  [[nodiscard]] float GetSpeed() const;
  [[nodiscard]] int GetDirectionChangeFactor() const;
  [[nodiscard]] float GetShell() const;
  [[nodiscard]] float GetNutritionValue() const override;
  [[nodiscard]] float GetBaseNutritionValue() const;
  [[nodiscard]] bool IsDead() const;
  [[nodiscard]] bool IsAlive() const;
  [[nodiscard]] bool IsDeleted() const;
  [[nodiscard]] bool HasEnergyToDivide() const;
  [[nodiscard]] bool DivisionCooldownPassed() const;
  [[nodiscard]] std::optional<core::Vector2<float>> GetDirection() const;
  [[nodiscard]] bool IsHungry() const;
  [[nodiscard]] float GetDivisionEnergy() const;
  [[nodiscard]] bool IsHunter() const;
  [[nodiscard]] bool IsNonHunter() const;
  [[nodiscard]] bool HasShell() const;
  [[nodiscard]] float GetPunchStrength() const;
  [[nodiscard]] bool HasDecayed() const;
  [[nodiscard]] bool HasTargetFood() const;
  [[nodiscard]] float GetShellThickness() const;
  [[nodiscard]] float GetHuntingSpeed() const;
  [[nodiscard]] float GetIdleSpeed() const;
  [[nodiscard]] sf::CircleShape &GetShape();
  [[nodiscard]] float GetEnergy() const;
  [[nodiscard]] bool IgnoresHunterNearFood() const;
  [[nodiscard]] bool IgnoresFoodNearHunter() const;
  [[nodiscard]] float GetClanValue() const;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
