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
          0.3,
          3,
          50
      };
    case HUNTING_SPEED:
      return {
          6.0,
          1.5,
          0.1,
          30
      };
    case IDLE_SPEED:
      return {
          3.0,
          1.5,
          0.1,
          30
      };
    case ENERGY:
      return {
          10,
          0.7,
          5,
          50
      };
    case HARD_SHELL:
      return {
          0.7,
          0.8,
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