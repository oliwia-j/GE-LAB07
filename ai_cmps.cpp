#include <cmath>
#include "ai_cmps.hpp"
#include "game_parameters.hpp"
#include <SFML/System.hpp>

using param = Parameters;

SteeringOutput SteeringBehaviours::seek(const sf::Vector2f& target, const sf::Vector2f& self) {
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    SteeringOutput steering;
    steering.direction = target - self;
    steering.direction = steering.direction / length(steering.direction);
    steering.rotation = 0.0f;
    return steering;
}


SteeringOutput SteeringBehaviours::flee(const sf::Vector2f& target, const sf::Vector2f& self) {
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    SteeringOutput steering;
    steering.direction = self - target;
    steering.direction = steering.direction / length(steering.direction);
    steering.rotation = 0.0f;
    return steering;
}



void SteeringComponent::update(const float& dt) {

    auto distance = [](const sf::Vector2f& a, const sf::Vector2f& b) -> float {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        };
    // If target (player) is more than 100 pixels away seek
    if (distance(_parent->get_position(), _player->get_position()) > 100.0f) {
        SteeringOutput output = SteeringBehaviours::seek(_player->get_position(), _parent->get_position());
        move(output.direction * _max_speed * dt);
    }
    // If target (player) is less than 50 pixels away flee
    else if (distance(_parent->get_position(), _player->get_position()) <
        50.0f) {
        SteeringOutput output = SteeringBehaviours::flee(_player->get_position(), _parent->get_position());
        move(output.direction * _max_speed * dt);
    }
}

SteeringComponent::SteeringComponent(Entity* p, Entity* player, float max_speed)
    : _player(player), _max_speed(max_speed), Component(p) {}

bool SteeringComponent::valid_move(const sf::Vector2f& pos) const {
    if (pos.x < 0.0f || pos.x > param::game_width ||
        pos.y < 0.0f || pos.y > param::game_height) {
        return false;
    }
    return true;
}

void SteeringComponent::move(const sf::Vector2f& p) {
    sf::Vector2f new_pos = _parent->get_position() + p;
    if (valid_move(new_pos)) {
        _parent->set_position(new_pos);
    }
}

void SteeringComponent::move(float x, float y) {
    move(sf::Vector2f(x, y));
}