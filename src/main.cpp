#include "CellsEvo/app.h"

// check that when multiple cells attack one they all hit
// depend on time, not on fps?
// photosynthetic cells
// refactoring
// optimizations, especially collision and food detection
// export/import world state
// runtime configuration
// charts of number of cells, and gene values
// as an option you can add a specific place where the food is produced constantly
// there will be a colony of cells and different species can fight for it

int main() {
  cells_evo::App(
      25,
      8,
      35,
      1500,
      800,
      60,
      0.3
  ).Run();
  return 0;
}
