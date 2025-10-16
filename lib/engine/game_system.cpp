#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Renderer.hpp"

using param = Parameters;

std::shared_ptr<Scene> GameSystem::_active_scene;

void GameSystem::start(unsigned int width, unsigned int height,
    const std::string& name, const float& time_step) {

    sf::RenderWindow window(sf::VideoMode({ width, height }), name);
    Renderer::initialise(window);
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
        _render();

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

void GameSystem::_init() {}

void GameSystem::_update(const float& dt) {
    _active_scene->update(dt);
    Renderer::update(dt);
}

void GameSystem::_render() {
    _active_scene->render();
    Renderer::render();
}

void Scene::update(const float &dt) {
    em->update(dt);
}

void Scene::render() {
    em->render();
}

void Scene::unload() {
    for (std::shared_ptr<Entity>& ent : em->list)
        ent.reset();
    em->list.clear();
}