#include "CellsEvo/app.h"

// try to use unique ptr where possible and pass it to functions as a reference
// optimizations for collision and food detection, keep cell arrays sorted between calls
// large cells should eat only large, it's not profitable to eat small, maybe gene?
// and probably they should prefer without shell, and prefer alive over dead
// implement the rest of genes
// large pieces of food, cells can stick to it while eating
// hunter-killers?
// calcium food type for shell
// different food types, food chains
// cells might stick on contact, number of connections might be limited depending on a gene
// cells might stick to a specific type of cell
// run collision detection more often then rendering
// features to distinguish cell types, like red is hunter, run from reds. There will be some impostors
// add gene eat everyone
// runtime configuration
// depend on time, not on fps?
// photosynthetic cells
// export/import world state
// as an option you can add a specific place where the food is produced constantly
// there will be a colony of cells and different species can fight for it

int main() {
  cells_evo::App(
      200,
      50,
      1000,
      2400,
      2400,
      60,
      20.2
  ).Run();
  return 0;
}
