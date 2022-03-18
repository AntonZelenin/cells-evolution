#ifndef CELLS_EVOLUTION_WORLD_H
#define CELLS_EVOLUTION_WORLD_H

#include <vector>
#include <map>
#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/food.h"
#include "tools.h"

namespace cells_evo::core {
    class World {
        std::vector<std::vector<bool>> field{};
        IndexDriver index_driver{};
    public:
        std::map<int, Food> food{};
        std::map<int, Cell> cells{};
        int width;
        int height;

        World();
        void AddCell(Cell cell);
        void AddFood(Food food);
        void AddFood(const std::vector<Food>& foods);
        void AddCells(const std::vector<Cell>& new_cells);
    };

    core::Entity* FindNearest(Position position, std::vector<core::Entity>);
}

#endif //CELLS_EVOLUTION_WORLD_H
