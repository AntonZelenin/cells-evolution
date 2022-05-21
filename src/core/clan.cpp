#include "CellsEvo/core/clan.h"

namespace cells_evo::core {
Clan GetClan(float value) {
  float num_of_clans = 4, clan_one_part = 1 / num_of_clans;
  if (value >= 0.f && value < clan_one_part) {
    return FIRST;
  } else if (value >= clan_one_part && value < 2.f * clan_one_part) {
    return SECOND;
  } else if (value >= 2.f * clan_one_part && value < 3.f * clan_one_part) {
    return THIRD;
  } else if (value >= 3.f * clan_one_part && value <= 4.f * clan_one_part) {
    return FOURTH;
  } else {
    throw std::runtime_error("Value of clan is invalid");
  }
}
}