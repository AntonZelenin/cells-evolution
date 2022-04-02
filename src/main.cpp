#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      10,
      3,
      20,
//      1,
//      0,
//      35,
      2000,
      1500,
      60,
      1.0
  ).Run();
  return 0;
}
