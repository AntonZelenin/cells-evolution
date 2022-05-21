#include "CellsEvo/app/app.h"

// draw food deposits
// maybe I should restrict max number of genes?
// add punch to all cells, hunters are stronger, nonhunters are weaker
// Add decision fight or run. Hunters will decide based on hunger as well
// add the nearby world state structure
// and add different clans of cells like green and yellow green
// like two clans of non hunters and they fight with each other, and they could stick together
// if cells can fight and stick to friends, they will be able to build colonies
// but I'll need to have a way to manage food for large groups
// add killing and splitting killed cells into food
// collisions with food, try to find a way to stop near food? you don't need to run if you reached it
// add hunting gene, without it cells will only eat dead
// add cell generation setup menu
// add change speed buttons
// use Fast Inverse Square Root instead of square root
// large cells should eat only large, it's not profitable to eat small, maybe gene?
// and probably they should prefer without shell, and prefer alive over dead
// large pieces of food, cells can stick to it while eating
// different food types, food chains
// cells might stick on contact, number of connections might be limited depending on a gene
// impostors?
// add gene eat everyone. Those will be hunters who attack friends, will call them killers
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
      2.0
  ).Run();
  return 0;
}
