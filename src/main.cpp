#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      20,
      3,
      30,
//      1,
//      0,
//      35,
      2000,
      1500,
      60,
      2.0
  ).Run();
  return 0;
}
