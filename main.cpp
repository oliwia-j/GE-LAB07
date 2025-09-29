#include <sstream>
#include <SFML/Graphics.hpp>

const float time_step = 0.017f; //60 fps
const int game_width = 800;
const int game_height = 600;

void init() {
    // Initialise game
}

void update(float dt) {
    // Handle updates
}

void render(sf::RenderWindow& window) {
    // Render window
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ game_width, game_height }), "GE_LAB");
    window.setVerticalSyncEnabled(true);
    init();

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
        update(dt);
        render(window);
        // Wait for the time_step to finish before displaying the next frame.
        sf::sleep(sf::seconds(time_step));
        // Wait for Vsync
        window.display();
    }
    return 0;
}
