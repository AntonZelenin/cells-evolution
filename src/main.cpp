#include "CellsEvo/app.h"

// dead cells should turn into food with time
// check that when multiple cells attack one they all hit
// depend on time, not on fps?
// store rand generators
// photosynthetic cells
// refactoring
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
      25,
      8,
      35,
      3000,
      1900,
      60,
      0.3
  ).Run();
  return 0;
}
