#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_

#include "geometry.h"
#include "types.h"

namespace cells_evo::core {
class Entity {
 public:
  virtual Position &GetPosition() = 0;
  [[nodiscard]] virtual uint GetId() const = 0;
  [[nodiscard]] virtual float GetSize() const = 0;
};

class EdibleEntity : public Entity {
 public:
  [[nodiscard]] virtual float GetNutritionValue() const = 0;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_
