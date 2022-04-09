#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      10,
      10,
      25,
//      1,
//      0,
//      35,
      2500,
      1800,
      60,
      2.5
  ).Run();
  return 0;
}
