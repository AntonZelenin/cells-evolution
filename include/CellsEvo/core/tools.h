#ifndef CELLS_EVOLUTION_TOOLS_H
#define CELLS_EVOLUTION_TOOLS_H


namespace cells_evo::core {
    class IndexDriver {
        unsigned int idx_ = 1;
    public:
        unsigned int GetNextId();
    };
}

#endif //CELLS_EVOLUTION_TOOLS_H
