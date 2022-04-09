#include "CellsEvo/core/app.h"

int main() {
  cells_evo::App(
      10,
      3,
      15,
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
