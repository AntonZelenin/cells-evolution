#include "CellsEvo/app.h"

// food on place of a dead cell
// refactoring
// large cells should repel smaller cells
// larger field
// optimizations, especially collision detection, food detection
// export/import world state
// implement genes that are not active yet
// seasonality
// runtime configuration
// charts of number of cells, and gene values
// fps is not working

int main() {
  cells_evo::App(
      20,
      5,
      5,
      3000,
      1900,
      60,
      0.5
  ).Run();
  return 0;
}
