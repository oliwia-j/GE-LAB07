#include "Player.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"

using namespace sf;
using namespace std;

using param = Parameters;
using ls = LevelSystem;

Player::Player() {};

void Player::update(const float& dt) {
    Entity::update(dt);
}

void Player::render() {
    Entity::render();
}

bool Player::valid_move(sf::Vector2f pos) {
    return (ls::get_tile_at(pos) != ls::WALL);
}