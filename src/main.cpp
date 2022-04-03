#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      15,
      3,
      20,
//      1,
//      0,
//      35,
      2500,
      1800,
      60,
      3.0
  ).Run();
  return 0;
}
