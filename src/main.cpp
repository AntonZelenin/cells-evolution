#include "CellsEvo/core.h"

int main() {
  cells_evo::App(
      15,
      1,
      10,
      2000,
      1500
  ).Run();
  return 0;
}
