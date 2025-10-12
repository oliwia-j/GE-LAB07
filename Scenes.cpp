#include "Scenes.hpp"
#include "Player.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"

using ls = LevelSystem;

using param = Parameters;

std::shared_ptr<Scene> Scenes::maze;

void MazeScene::update(const float& dt) {
    Scene::update(dt);
}

void MazeScene::render(sf::RenderWindow& window) {
    ls::render(window);
    Scene::render(window);
}

void MazeScene::load() {
    std::shared_ptr<Entity> player = std::make_shared<Player>();
    _entities.push_back(player);
    reset();
}
void MazeScene::reset() {
    ls::load_level(_file_path);
    for (int y = 0; y < ls::get_height(); ++y) {
        for (int x = 0; x < ls::get_width(); ++x) {
            std::cout << ls::get_tile({ x, y });
        }
        std::cout << std::endl;
    }

    _entities[0]->set_position({param::game_width / 2, param::game_height / 2 });
}

void MazeScene::set_file_path(const std::string& file_path) {
    _file_path = file_path;
}