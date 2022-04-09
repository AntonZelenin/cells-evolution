#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_GENES_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_GENES_H_

#include <unordered_map>
#include "CellsEvo/core/tools.h"

namespace cells_evo::genetics {
enum GeneType {
  DIRECTION_CHANGE_FACTOR,
  DIVISION_COOLDOWN,
  ENERGY_TO_DIVIDE,
  HARD_SHELL,
  PREFER_FOOD_OVER_HUNTER,
  AVOID_HUNTERS,
  SIZE,
  SPEED,
  ENERGY,
  STICK_TO_FRIEND,
  AVOID_FRIENDS,
};

struct Gene {
  float value;
  GeneType type;
};

struct GeneConfig {
  float base_value;
  float base_deviation;
  float min;
  float max;
};

GeneConfig BuildGeneConfig(GeneType gene_type);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_GENES_H_
