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
      1800,
      60,
      3.5
  ).Run();
  return 0;
}
