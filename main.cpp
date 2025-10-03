#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "game_system.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({ Parameters::game_width, Parameters::game_height }), "GE_LAB");
    window.setVerticalSyncEnabled(true);
    GameSystem::init();

    while (window.isOpen()) {
        // Reset clock, recalculate deltatime
        static sf::Clock clock;
        float dt = clock.restart().asSeconds();

        // Check and consume events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }
        // Quit Via ESC Key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        GameSystem::update(dt);
        GameSystem::render(window);
        // Wait for the time_step to finish before displaying the next frame
        sf::sleep(sf::seconds(Parameters::time_step));
        // Wait for Vsync
        window.display();
    }
    return 0;
}
