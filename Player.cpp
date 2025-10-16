#include "Player.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"
#include "Renderer.hpp"

using namespace sf;
using namespace std;

using param = Parameters;
using ls = LevelSystem;

Player::Player()
    : Entity(make_unique<CircleShape>(_radius)) {
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(sf::Vector2f(_radius, _radius));
}

void Player::update(const float& dt) {
    // Move in four directions based on keys
    sf::Vector2f movement(0.f, 0.f);
    sf::Vector2f new_pos(0.f, 0.f);
    bool in_bounds = true;

    // Move Left
    if (sf::Keyboard::isKeyPressed(param::controls[0])) {
        movement = Vector2f({ dt * -1 * _speed, 0.0f });
        new_pos = get_position() + movement;
        in_bounds = get_position().x > _radius;
    }
    // Move Right
    if (sf::Keyboard::isKeyPressed(param::controls[1])) {
        movement = Vector2f({ dt * _speed, 0.0f });
        new_pos = get_position() + movement;
        in_bounds = get_position().x < param::game_width - _radius;
    }
    // Move Up
    if (sf::Keyboard::isKeyPressed(param::controls[2])) {
        movement = Vector2f({ 0.0f, dt * -1 * _speed });
        new_pos = get_position() + movement;
        in_bounds = get_position().y > _radius;
    }
    // Move Down
    if (sf::Keyboard::isKeyPressed(param::controls[3])) {
        movement = Vector2f({ 0.0f, dt * _speed });
        new_pos = get_position() + movement;
        in_bounds = get_position().y < param::game_height - _radius;
    }

    if (in_bounds && valid_move(new_pos)) {
        move(movement);
    }
    Entity::update(dt);
}

void Player::render() const {
    Renderer::queue(_shape.get());
}

bool Player::valid_move(sf::Vector2f pos) {
    return (ls::get_tile_at(pos) != ls::WALL);
}