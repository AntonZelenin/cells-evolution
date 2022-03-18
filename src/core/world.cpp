#include "CellsEvo/core/world.h"

namespace cells_evo::core {
    World::World() {
        this->width = 1920;
        this->height = 1080;
        this->field.reserve(this->height);
        for (int i = 0; i < this->width; i++) {
            std::vector<bool> v(this->width, false);
            this->field.push_back(v);
        }
        this->AddFood(FoodGenerator().GetFirstGeneration(
                this->width,
                this->height,
                20
        ));
        this->AddCells(CellGenerator().GetFirstGeneration(
                this->width,
                this->height,
                5
        ));
    }

    void World::AddFood(Food new_food) {
        // todo this will not work
        this->field[new_food.GetPosition().X()][new_food.GetPosition().Y()] = true;
        unsigned int id = this->index_driver.GetNextId();
        new_food.SetId(id);
        this->food.insert({id, new_food});
    }

    void World::AddCell(Cell cell) {
        // todo this will not work
        this->field[cell.GetPosition().X()][cell.GetPosition().Y()] = true;
        unsigned int id = this->index_driver.GetNextId();
        cell.SetId(id);
        this->cells.insert({id, cell});
    }

    void World::AddFood(const std::vector<Food>& foods) {
        // todo will it work as I expect?
        for (auto f: foods) {
            this->AddFood(f);
        }
    }

    void World::AddCells(const std::vector<Cell>& new_cells) {
        for (auto c: new_cells) {
            this->AddCell(c);
        }
    }

    void World::Tick() {

    }
}
