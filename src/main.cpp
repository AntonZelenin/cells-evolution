#include "CellsEvo/app.h"

// gui, number of cells, gene values
// implement the rest of genes
// large pieces of food, cells can stick to it while eating
// hunter-killers
// different food types, food chains
// cells might stick on contact, number of connections might be limited depending on a gene
// cells might stick to a specific type of cell
// run collision detection more often then rendering
// features to distinguish cell types, like red is hunter, run from reds. There will be some impostors
// add gene eat everyone
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
      2000,
      1800,
      60,
      0.3
  ).Run();
  return 0;
}
