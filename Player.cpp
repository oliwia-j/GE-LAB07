#include "Player.hpp"
#include "game_parameters.hpp"

using namespace sf;
using namespace std;

using param = Parameters;

Player::Player()
    : Entity(make_unique<CircleShape>(_radius)) {
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(sf::Vector2f(_radius, _radius));
}

void Player::update(const float& dt) {
    // Move in four directions based on keys
    // Move Left
    if (sf::Keyboard::isKeyPressed(param::controls[0]) &&
        get_position().x > _radius * 2 / 2.f) {
        move({ dt * -1 * _speed, 0.0f });
    }
    // Move Right
    if (sf::Keyboard::isKeyPressed(param::controls[1]) &&
        get_position().x < param::game_width - _radius*2 / 2.f) {
        move({ dt * _speed, 0.0f });
    }
    // Move Up
    if (sf::Keyboard::isKeyPressed(param::controls[2]) &&
        get_position().y < -(_radius * 2) / 2.f) {
        move({ 0.0f, dt * -1 * _speed });
    }
    // Move Down
    if (sf::Keyboard::isKeyPressed(param::controls[3]) &&
        get_position().y > -(param::game_height - _radius * 2) / 2.f) {
        move({ 0.0f, dt * _speed });
    }
    Entity::update(dt);
}

void Player::render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}