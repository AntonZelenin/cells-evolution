#include "CellsEvo/app.h"

// add hunting gene, without it cells will only eat dead
// add killing and organic food type
// add cell generation setup menu
// add change speed buttons
// use Fast Inverse Square Root instead of square root
// large cells should eat only large, it's not profitable to eat small, maybe gene?
// and probably they should prefer without shell, and prefer alive over dead
// large pieces of food, cells can stick to it while eating
// different food types, food chains
// cells might stick on contact, number of connections might be limited depending on a gene
// cells might stick to a specific type of cell
// hunter impostors
// add gene eat everyone
// photosynthetic cells
// export/import world state
// as an option you can add a specific place where the food is produced constantly

int main() {
  cells_evo::App(
      30,
      8,
      40,
      3000,
      2000,
      60,
      0.8
  ).Run();
  return 0;
}
