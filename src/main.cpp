#include "CellsEvo/core.h"

int main() {
  cells_evo::App(
      10,
      30,
      2500,
      1500
  ).Run();
  return 0;
}
