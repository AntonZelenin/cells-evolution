#include "CellsEvo/core/logic/cell_logic.h"

namespace cells_evo::logic {
bool NonHunterCellLogic::CellGotFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).Magnitude() < (cell.GetRadius() + food_entity.GetRadius());
}

core::Vector2<float> NonHunterCellLogic::GetRandomDirection(core::Cell &cell) {
  if (!cell.GetDirection() || cell.lifetime_ % cell.GetDirectionChangeFactor() == 0) {
    auto coords = core::GetRandomFloats(-1, 1, 2);
    auto direction = core::Vector2<float>(coords[0], coords[1]);
    direction.Normalize();
    cell.SetDirection(direction);
    return direction;
  }
  return cell.GetDirection().value();
}
}
