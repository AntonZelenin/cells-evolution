#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_

#include "cell.h"
#include "world.h"

namespace cells_evo::logic {
class NonHunterLogic {

};

class HunterLogic {

};

class Logic {
  core::World &world_;
  unsigned int world_ticks_ = 0;
  unsigned int food_production_rate_;
  std::map<unsigned int, unsigned int> cell_food_cache_;
  std::map<unsigned int, unsigned int> hunter_cell_cell_cache_;

  HunterLogic hunter_logic_;
  NonHunterLogic non_hunter_logic_;

  std::optional<core::Food> FindClosestFood(core::Cell &cell);

  std::optional<core::Cell> FindClosestCell(core::Cell &cell);

  void CountTick();

  void MoveCells();

  static bool CouldSensedFood(core::Cell &cell, core::Food &food);

  static core::Vector2<float> GetRandomSinDirection();

  void MoveHunterCells();

  void Eat();

  void RebuildCellsFoodCache();

  static bool CellGotFood(core::Cell &cell, core::Food &food);

  static bool HunterCellGotCell(core::Cell &hunter_cell, core::Cell &cell);

  void RemoveFoodFromCache(const core::Food &food);

  void ProcessEatFood(core::Cell &cell);

  void ProcessEatCell(core::Cell &hunter_cell);

  void GenerateFood();

  void DivideCells();

  static core::Cell DivideCell(core::Cell &cell);

  void CheckCellsEnergy();

  void KillCell(unsigned int cell_id);

 public:
  explicit Logic(core::World &world, unsigned int food_production_rate);

  void WorldTick();

  core::Vector2<float> ChooseDirection(core::Cell &cell);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
