#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      25,
      5,
      35,
//      1,
//      0,
//      35,
      2500,
      2050,
      60,
      3.0
  ).Run();
  return 0;
}
