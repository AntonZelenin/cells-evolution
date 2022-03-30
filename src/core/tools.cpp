#include "CellsEvo/core/tools.h"


namespace cells_evo::core {
unsigned int IndexDriver::GetNextId() {
  return idx_++;
}

std::vector<int> GetRandomInts(int from, int  to, unsigned int number) {
  std::vector<int> numbers{};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(from, to);

  for (; number > 0; number--) {
    numbers.push_back(distribution(generator));
  }

  return numbers;
}

std::vector<float> GetRandomFloats(float from, float  to, unsigned int number) {
  std::vector<float> numbers{};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution(from, to);

  for (; number > 0; number--) {
    numbers.push_back(distribution(generator));
  }

  return numbers;
}
}
