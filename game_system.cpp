#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

sf::Texture gs::spritesheet;
std::vector<std::shared_ptr<Ship>> gs::ships;

void GameSystem::reset() {
	Invader::speed = 20.f;
}

void GameSystem::init() {
    if (!spritesheet.loadFromFile("./Debug/res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

	/*
	for (int i = 1; i <= param::invaders_cols; ++i) {
		std::shared_ptr<Invader> inv = std::make_shared<Invader>(sf::IntRect(sf::Vector2i(i % 2  == 0 ? param::sprite_size : 0, 0), sf::Vector2i(param::sprite_size, param::sprite_size)), sf::Vector2f(i*100, 100));
		ships.push_back(inv); // This is when the copy constructor is called
	}
	*/

	gs::reset();

	std::shared_ptr<Player> player = std::make_shared<Player>();
	ships.push_back(player);

	for (int r = 1; r <= param::invaders_rows; ++r) {
		auto rect = sf::IntRect(sf::Vector2i(r % 2 == 0 ? param::sprite_size : 0, 0),sf::Vector2i(param::sprite_size, param::sprite_size));
		for (int c = 1; c <= param::invaders_cols; ++c) {
			sf::Vector2f pos = sf::Vector2f(c * 100, r * 100);
			std::shared_ptr<Invader> inv = std::make_shared<Invader>(rect, pos);
			ships.push_back(inv); // This is when the copy constructor is called
		}
	}

}

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

void GameSystem::clean() {
	for (std::shared_ptr<Ship>& ship : ships) {
		ship.reset(); // Free up the memory of this shared pointer
	}
	ships.clear(); // Clear the vector to be sure we free up any memory left
}