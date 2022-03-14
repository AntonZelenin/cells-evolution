#ifndef CELLS_EVOLUTION_APP_H
#define CELLS_EVOLUTION_APP_H


#include "SFML/Graphics/RenderWindow.hpp"

class App {
    // todo for future: can I hide private members from headers?
    sf::RenderWindow* window;
public:
    App();
    ~App();

    void run();
};


#endif //CELLS_EVOLUTION_APP_H
