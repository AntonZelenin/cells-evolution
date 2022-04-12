#include "CellsEvo/core/genetics/genes.h"

namespace cells_evo::genetics {
GeneConfig BuildGeneConfig(GeneType gene_type) {
  // todo it should be configurable in something like yaml
  switch (gene_type) {
    case DIRECTION_CHANGE_FACTOR:
      return {
          200,
          20,
          0.1,
          1000
      };
    case SIZE:
      return {
          5,
          0.5,
          4,
          100
      };
    case SPEED:
      return {
          0.34,
          0.4,
          0.05,
          6
      };
    case ENERGY:
      return {
          10,
          0.4,
          5,
          50
      };
    case HARD_SHELL:
      return {
          0,
          0.1,
          0,
          10
      };
    case ENERGY_TO_DIVIDE:
      return {
          10,
          0.3,
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
    case AVOID_HUNTERS:
    case PREFER_FOOD_OVER_HUNTER:
    case STICK_TO_FRIEND:
    case AVOID_FRIENDS:
      return {
          0.5,
          0.1,
          0,
          1
      };
  }
}
}