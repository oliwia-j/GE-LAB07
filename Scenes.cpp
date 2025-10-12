#include "Scenes.hpp"
#include "Player.hpp"
#include "game_parameters.hpp"

using param = Parameters;

std::shared_ptr<Scene> Scenes::maze;

void MazeScene::update(const float& dt) {
    Scene::update(dt);
}

void MazeScene::render(sf::RenderWindow& window) {
    Scene::render(window);
}

void MazeScene::load() {
    std::shared_ptr<Entity> player = std::make_shared<Player>();
    _entities.push_back(player);
    reset();
}
void MazeScene::reset() {
    _entities[0]->set_position({param::game_width / 2, param::game_height / 2 });
}

void MazeScene::set_file_path(const std::string& file_path) {
    _file_path = file_path;
}