#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_GENETICS_GENES_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_GENETICS_GENES_H_

namespace cells_evo::genetics {
enum GeneType {
  DIRECTION_CHANGE_FACTOR,
  DIVISION_COOLDOWN,
  ENERGY_TO_DIVIDE,
  HARD_SHELL,
  IGNORE_FOOD_NEAR_HUNTER,
  IGNORE_HUNTER_NEAR_FOOD,
  IGNORE_HUNTER_IS_RECESSIVE,
  SIZE,
  HUNTING_SPEED,
  IDLE_SPEED,
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
  float deviation;
  float min;
  float max;
};

GeneConfig BuildGeneConfig(GeneType gene_type);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_GENETICS_GENES_H_
