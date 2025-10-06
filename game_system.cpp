#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

sf::Texture gs::spritesheet;

void GameSystem::init() {
    if (!spritesheet.loadFromFile("./Debug/res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
}

void GameSystem::reset() {
}

void GameSystem::update(const float& dt) {
}

void GameSystem::render(sf::RenderWindow& window) {
}

void GameSystem::clean() {
}