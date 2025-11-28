#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "game_parameters.hpp"
#include "scenes.hpp"

using param = Parameters;

int main() {
    Physics::initialise();
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::menu->load();
    Scenes::level = std::make_shared<LevelScene>();
    Scenes::level->load();
    GameSystem::set_active_scene(Scenes::menu);
    GameSystem::start(param::game_width, param::game_height, "Platformer", Physics::time_step, true);
    Physics::shutdown();
    return 0;
}