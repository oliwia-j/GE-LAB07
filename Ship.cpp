#include "ship.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"

// Renaming with using holds only for the current scope (here the whole file)
using param = Parameters; // Renaming the struct Parameters into param to have a more compact and readable code
using gs = GameSystem; // Renaming the struct GameSystem into gs to have a more compact and readable code

bool Invader::direction;
float Invader::speed;

Ship::Ship() {};

Ship::Ship(const Ship& s) :
	_sprite(s._sprite) {}

Ship::Ship(sf::IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(GameSystem::spritesheet);
	setTextureRect(_sprite);
};

void Ship::move_down() {}

void Ship::update(const float& dt) {}

// Define the ship deconstructor
// Although we set this to pure virtual, we still have to define it
Ship::~Ship() = default;

Invader::Invader() : Ship() {}
Invader::Invader(const Invader& inv) : Ship(inv) {}
Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
	setOrigin(sf::Vector2f(16.f, 16.f));;
	setPosition(pos);
}

void Invader::move_down() {
	move({ 0, param::invaders_step });
}

void Invader::update(const float& dt) {
	Ship::update(dt);

	move(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f);

	if ((direction && getPosition().x > param::game_width - param::sprite_size / 2.f) ||
		(!direction && getPosition().x < param::sprite_size / 2.f)) {
		direction = !direction;
		speed += param::invaders_speedup;
		for (std::shared_ptr<Ship>& ship : gs::ships) {
			ship->move_down();
		}
	}
}

Player::Player() : Ship::Ship(sf::IntRect(
										sf::Vector2i(param::sprite_size * 5, param::sprite_size),
										sf::Vector2i(param::sprite_size, param::sprite_size))) {
	setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
	setPosition(param::game_width / 2.f, param::game_height - static_cast<float>(param::sprite_size));
}

void Player::update(const float& dt) {
	Ship::update(dt);

	if (sf::Keyboard::isKeyPressed(param::controls[0]) &&
		getPosition().x > param::sprite_size / 2.f) {
		move(dt * -1 * param::p_speed, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(param::controls[1]) &&
		getPosition().x < param::game_width - param::sprite_size / 2.f) {
		move(dt * param::p_speed, 0.0f);
	}
}