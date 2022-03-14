#include "CellsEvo/Core.h"
#include "CellsEvo/Graphics.h"
#include "SFML/Graphics.hpp"

void App::run() {
    Cell cell = Cell();
    while (this->window->isOpen()) {
        sf::Event event{};
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window->close();
        }
        this->window->clear(sf::Color::Black);
        this->window->draw(CellDrawer().get(&cell));
        this->window->display();
    }
}

App::App() {
    // todo provide configuration?
    this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Cells");
    window->setVerticalSyncEnabled(true);
}

App::~App() {
    delete this->window;
}
