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
  const float k_energy_consumption_coefficient_ = 0.001;
  const float k_division_energy_threshold_ = 10;
  const float k_division_energy_size_coefficient_ = 0.1;

  Position position_;

 public:
  constexpr static const float k_default_energy_value_ = 10;
  constexpr static const float k_max_distance_food_detection_ = 200;

  // todo everything is public =*(
  // this is the energy the cell will give if eaten
  float energy_{};
  unsigned int id_{};
  float size_{};
  float speed_{};
  Type type_;
  std::unordered_map<genetics::GeneType, genetics::Gene> genes_{};

  Cell(
      float size,
      float speed,
      float energy,
      Type type,
      Position position,
      std::vector<genetics::Gene> const &genes
  );
//  Cell(const Cell &cell);

  Position &GetPosition() override;
  float GetSize() override;
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

  int GetDirectionChangeFactor() {
    return static_cast<int>(genes_.find(genetics::GeneType::DIRECTION_CHANGE_FACTOR)->second.value);
  }
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CELL_H_
