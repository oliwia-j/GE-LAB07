#include "game_system.hpp"
#include "game_parameters.hpp"

sf::Texture GameSystem::spritesheet;
std::vector<std::shared_ptr<Ship>> GameSystem::ships;

void GameSystem::init() {
    if (!spritesheet.loadFromFile("./Debug/res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

	for (int i = 1; i <= Parameters::invaders_count; ++i) {
		std::shared_ptr<Invader> inv = std::make_shared<Invader>(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)), sf::Vector2f(i*100, 100));
		ships.push_back(inv); // This is when the copy constructor is called
	}
}

void GameSystem::clean() {}

void GameSystem::update(const float& dt) {
	for (std::shared_ptr<Ship>& s : ships) {
		s->update(dt);
	}
}

void GameSystem::render(sf::RenderWindow& window) {
	for (const std::shared_ptr<Ship>& s : ships) {
		window.draw(*(s.get()));
	}
}