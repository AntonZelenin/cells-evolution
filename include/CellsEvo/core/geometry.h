#ifndef CELLS_EVOLUTION_GEOMETRY_H
#define CELLS_EVOLUTION_GEOMETRY_H

struct Position {
    int x{};
    int y{};

    Position() = default;

    // todo another construct implementation?
    Position(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

#endif //CELLS_EVOLUTION_GEOMETRY_H
