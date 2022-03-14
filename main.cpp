#include <SFML/Graphics.hpp>
#include <map>

enum CellType {
    hunter, nonhunter
};

struct Position {
    int x{};
    int y{};

    Position() = default;

    // todo
    Position(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct Cell {
    int size;
    int speed;
    CellType type;

    Position position;

    Cell() {
        this->size = 20.f;
        this->speed = 1.f;
        this->type = CellType::nonhunter;
        this->position = Position(100, 100);
    }
};

class CellShape : public sf::CircleShape {
    using CircleShape::CircleShape;
};

class CellColorProvider {
    std::map<CellType, sf::Color> mapping{
            {CellType::hunter, sf::Color::Red},
            {CellType::nonhunter, sf::Color::Green},
    };
public:
    sf::Color get(const CellType* type) {
        return this->mapping[*type];
    }
};

class CellDrawer {
    float THICKNESS_COEFFICIENT = 5.f;

    CellColorProvider color_provider = CellColorProvider();

    [[nodiscard]] float get_thickness(int size) const {
        return static_cast<float>(size) / this->THICKNESS_COEFFICIENT;
    }
public:
    CellShape get(const Cell* cell) {
        CellShape shape(static_cast<float>(cell->size));
//        shape.setFillColor();
        shape.setOutlineThickness(this->get_thickness(cell->size));
        shape.setOutlineColor(this->color_provider.get(&cell->type));
        return shape;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Cells");
    window.setVerticalSyncEnabled(true);

    Cell cell = Cell();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(CellDrawer().get(&cell));
        window.display();
    }

    return 0;
}
