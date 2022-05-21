#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CLAN_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CLAN_H_

#include <stdexcept>

namespace cells_evo::core {
enum Clan {
  FIRST,
  SECOND,
  THIRD,
  FOURTH,
};

Clan GetClan(float value);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_CLAN_H_
