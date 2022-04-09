#include "CellsEvo/core/app.h"

// refactoring
// larger field
// optimizations, especially collision detection, food detection
// food on place of a dead cell
// export/import world state
// implement genes that are not active yet
// food seasonality
// runtime configuration
// charts of number of cells, and gene values

int main() {
  cells_evo::App(
      20,
      5,
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
