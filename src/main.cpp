#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      20,
      5,
      25,
//      1,
//      0,
//      35,
      2500,
      1800,
      60,
      3.5
  ).Run();
  return 0;
}
