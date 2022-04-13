#include "CellsEvo/app.h"

// check that when multiple cells attack one they all hit
// depend on time, not on fps?
// photosynthetic cells
// refactoring
// larger field
// optimizations, especially collision detection, food detection
// export/import world state
// runtime configuration
// charts of number of cells, and gene values

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
