#include "Scenes.hpp"
#include "Player.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "level_system.hpp"

using ls = LevelSystem;
using param = Parameters;

std::shared_ptr<Scene> Scenes::maze;
std::shared_ptr<Scene> Scenes::end;

void MazeScene::update(const float& dt) {
    if (ls::get_tile_at(_entities[0]->get_position()) == ls::END) {
        if (_file_path == std::string(param::maze_1)) {
            _file_path = param::maze_2;
            reset();
        }
        else if (_file_path == std::string(param::maze_2)) {
            _file_path = param::maze_3;
            reset();
        }
        else {
            unload();
            GameSystem::set_active_scene(Scenes::end);
        }
        return;
    }

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

    _entities[0]->set_position(ls::get_start_position());
}

void MazeScene::set_file_path(const std::string& file_path) {
    _file_path = file_path;
}

void EndScene::load() {
    font.loadFromFile("./Debug/res/fonts/Roboto/static/Roboto-Bold.ttf");
    win_text.setFont(font);
    win_text.setFillColor(sf::Color::White);
    win_text.setString("You won!");
    win_text.setPosition((param::game_width * .5f) - (win_text.getLocalBounds().width * .5f), 100);
}

void EndScene::render(sf::RenderWindow& window) {
    Scene::render(window);
    window.draw(win_text);
}