#include "states.hpp"
#include "graphics_cmps.hpp"
#include <cmath>
#include <iostream>

void NormalState::execute(Entity* owner, const float& dt) {
    auto length = [](sf::Vector2f v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(sf::Color::Blue);

    if (length(owner->get_position() - _player->get_position()) < 100.0f) {
        std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
        sm->change_state("near");
    }
}

void NearState::execute(Entity* owner, const float& dt) {
    auto length = [](sf::Vector2f v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(sf::Color::Green);

    if (length(owner->get_position() - _player->get_position()) > 100.0f) {
        std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
        sm->change_state("normal");
    }
}

SteeringState::SteeringState(SteeringFunction steering, std::shared_ptr<Entity> player, float max_speed, sf::Color color)
    : _steering(steering), _player(player), _max_speed(max_speed), _color(color) {}

void SteeringState::execute(Entity* owner, const float& dt) {
    std::shared_ptr<ShapeComponent> s = owner->get_components<ShapeComponent>()[0];
    s->get_shape().setFillColor(_color);
    SteeringOutput output = _steering(_player->get_position(), owner->get_position());
    owner->set_position(owner->get_position() + (output.direction * _max_speed * dt));
}