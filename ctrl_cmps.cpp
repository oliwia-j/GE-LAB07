#include "ctrl_cmps.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"

using gs = GameSystem;
using param = Parameters;

// Checks keyboard and moves the player.
void KeyboardMovementComponent::update(const float& dt) {
    sf::Vector2f direction(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y += 1.0f;
    }

    move(direction * _speed * dt);
}

// Initialises the component.
KeyboardMovementComponent::KeyboardMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

// Checks if the proposed move is valid.
bool KeyboardMovementComponent::valid_move(const sf::Vector2f& pos) {
    if (pos.x < 0.0f || pos.x > param::game_width ||
        pos.y < 0.0f || pos.y > param::game_height) {
        return false;
    }
    return true;
}

// Moves the component's parent.
void KeyboardMovementComponent::move(const sf::Vector2f& p) {
    auto new_pos = _parent->get_position() + p;
    if (valid_move(new_pos)) {
        _parent->set_position(new_pos);
    }
}

// Moves the component's parent.
void KeyboardMovementComponent::move(float x, float y) {
    move(sf::Vector2f(x, y));
}