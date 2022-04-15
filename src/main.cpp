#include "CellsEvo/app.h"

// gui, number of cells, gene values
// runtime configuration
// optimizations, especially collision and food detection
// depend on time, not on fps?
// photosynthetic cells
// export/import world state
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
