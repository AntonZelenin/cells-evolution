#include "CellsEvo/core.h"


// they run away because they don't feel food and logic of movement is not complete
int main() {
  cells_evo::App(
      15,
      1,
      10,
      2000,
      1500,
      60,
      0.5
  ).Run();
  return 0;
}
