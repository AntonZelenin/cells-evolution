#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      20,
      10,
      25,
//      1,
//      0,
//      35,
      3000,
      1900,
      60,
      2.5
  ).Run();
  return 0;
}
