#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_

namespace cells_evo::core {
class IndexDriver {
  unsigned int idx_ = 1;
 public:
  unsigned int GetNextId();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_TOOLS_H_
