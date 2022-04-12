#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_

#include "geometry.h"

namespace cells_evo::core {
class Entity {
 public:
  virtual Position &GetPosition() = 0;
  virtual void SetPosition(Position) = 0;
  virtual unsigned int GetId() = 0;
  [[nodiscard]] virtual float GetRadius() const = 0;
};

class EdibleEntity : public Entity {
 public:
  [[nodiscard]] inline virtual float GetNutritionValue() const = 0;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_