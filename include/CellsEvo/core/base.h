#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_

#include "geometry.h"

namespace cells_evo::core {
// todo make abstract class with attributes?
class Entity {
 public:
  virtual Position &GetPosition() = 0;
  virtual void SetPosition(Position) = 0;
  virtual unsigned int GetId() = 0;
  [[nodiscard]] virtual float GetSize() const = 0;
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_BASE_H_
