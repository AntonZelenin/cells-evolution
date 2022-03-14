#ifndef CELLS_EVOLUTION_CELL_H
#define CELLS_EVOLUTION_CELL_H

// todo move
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

enum CellType {
    hunter, nonhunter
};

struct Cell {
    int size;
    int speed;
    CellType type;

    Position position;

    Cell();
};

#endif //CELLS_EVOLUTION_DRAWER_H
