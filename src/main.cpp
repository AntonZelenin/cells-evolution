#include "CellsEvo/core.h"

int main() {
  cells_evo::App(
      10,
      3,
      20,
//      1,
//      0,
//      0,
      2000,
      1500,
      60,
      0.0
  ).Run();
  return 0;
}
