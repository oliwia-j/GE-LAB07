#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;

std::shared_ptr<Scene> GameSystem::_active_scene;

void GameSystem::start(unsigned int width, unsigned int height,
    const std::string& name, const float& time_step) {

    sf::RenderWindow window(sf::VideoMode({ width, height }), name);
    _init();
    sf::Event event;
    while (window.isOpen()) {
        static sf::Clock clock;
        float dt = clock.restart().asSeconds();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                clean();
                return;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        window.clear();
        _update(dt);
        _render(window);
        // Wait for the time_step to finish before displaying the next frame
        sf::sleep(sf::seconds(time_step));
        // Wait for Vsync
        window.display();
    }
    window.close();
    clean();
}

void GameSystem::clean() {
}

void GameSystem::reset() {
}

void GameSystem::set_active_scene(const std::shared_ptr<Scene>& act_sc) {
    _active_scene = act_sc;
}

void GameSystem::_init() {
}

void GameSystem::_update(const float& dt) {
    _active_scene->update(dt);
}

void GameSystem::_render(sf::RenderWindow& window) {
    _active_scene->render(window);
}

void Scene::update(const float &dt) {
    for (std::shared_ptr<Entity>& ent : _entities) {
        ent->update(dt);
    }
}

void Scene::render(sf::RenderWindow& window) {
    for (std::shared_ptr<Entity>& ent : _entities) {
        ent->render(window);
    }
}

void Scene::unload() {
    for (std::shared_ptr<Entity>& ent : _entities)
        ent.reset();
    _entities.clear();
}