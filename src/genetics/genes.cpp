#include "CellsEvo/genetics/genes.h"

namespace cells_evo::genetics {
GeneConfig BuildGeneConfig(GeneType gene_type) {
  // todo it should be configurable in something like yaml
  switch (gene_type) {
    case DIRECTION_CHANGE_FACTOR:
      return {
          200,
          20,
          1,
          1000
      };
    case SIZE:
      return {
          5,
          0.8,
          3,
          50
      };
    case HUNTING_SPEED:
      return {
          2.0,
          0.5,
          0.1,
          5
      };
    case IDLE_SPEED:
      return {
          1.0,
          0.5,
          0.1,
          5
      };
    case HARD_SHELL:
      return {
          1.0,
          1.0,
          0,
          5
      };
    case ENERGY_TO_DIVIDE:
      return {
          15,
          0.5,
          5,
          100
      };
    case DIVISION_COOLDOWN:
      return {
          240,
          10,
          180,
          1000
      };
    case CLAN:
      return {
          0.5,
          0.5,
          0,
          1
      };
    case IGNORE_HUNTER_IS_RECESSIVE:
    case IGNORE_FOOD_NEAR_HUNTER:
    case IGNORE_HUNTER_NEAR_FOOD:
    case STICK_TO_FRIEND:
    case AVOID_FRIENDS:
      return {
          0.5,
          0.2,
          0,
          1
      };
  }
}
}