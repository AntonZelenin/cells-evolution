#include "CellsEvo/core/genetics/genes.h"

namespace cells_evo::genetics {
GeneConfig BuildGeneConfig(GeneType gene_type) {
  // todo it should be configurable in something like yaml
  switch (gene_type) {
    case DIRECTION_CHANGE_FACTOR:
      return {
          120,
          0.1,
          0,
          1000
      };
    case SIZE:
      return {
          5,
          0.1,
          0,
          100
      };
    case SPEED:
      return {
          1,
          0.1,
          0,
          150
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
          0.1,
          0,
          100
      };
    case DIVISION_COOLDOWN:
      return {
          120,
          0.1,
          120,
          1000
      };
    case RUN_FROM_HUNTER:
    case PREFER_FOOD_OVER_HUNTER:
      return {
          0.5,
          0.1,
          0,
          1
      };
  }
}
}