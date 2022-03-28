#include "CellsEvo/core.h"

int main() {
  cells_evo::App(
      10,
      3,
      30,
      2500,
      1800
  ).Run();
  return 0;
}
