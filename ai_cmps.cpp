#include <cmath>
#include "ai_cmps.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"

using param = Parameters;

SteeringFunction SteeringBehaviours::seek = [](const sf::Vector2f& target, const sf::Vector2f& self) -> SteeringOutput {
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    SteeringOutput steering;
    steering.direction = target - self;
    steering.direction = steering.direction / length(steering.direction);
    steering.rotation = 0.0f;
    return steering;
    };


SteeringFunction SteeringBehaviours::flee = [](const sf::Vector2f& target, const sf::Vector2f& self) -> SteeringOutput {
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    SteeringOutput steering;
    steering.direction = self - target;
    steering.direction = steering.direction / length(steering.direction);
    steering.rotation = 0.0f;
    return steering;
    };


SteeringFunction SteeringBehaviours::stationary = [](const sf::Vector2f& target, const sf::Vector2f& self) -> SteeringOutput {
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    SteeringOutput steering;
    steering.direction = { 0,0 };
    steering.rotation = 0.0f;
    return steering;
    };

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

void PathfindingComponent::update(const float& dt) {
    _elapsed += dt;
    if (_elapsed >= 0.1) {
        _elapsed = 0.0;
        if (_index < _path.size()) {
            float new_x = _path[_index].x * param::tile_size;
            float new_y = _path[_index].y * param::tile_size;
            _parent->set_position(sf::Vector2f(new_x, new_y));
            ++_index;
        }
    }
}

PathfindingComponent::PathfindingComponent(Entity* p) : Component(p) {}

void PathfindingComponent::set_path(std::vector<sf::Vector2i>& path) {
    _index = 0;
    _path = path;
}


StateMachineComponent::StateMachineComponent(Entity* p) : _current_state(nullptr), Component(p) { }

void StateMachineComponent::update(const float& dt)
{
    if (_current_state != nullptr) {
        _current_state->execute(_parent, dt);
    }
}

void StateMachineComponent::add_state(const std::string& name, std::shared_ptr<State> state)
{
    _states[name] = state;
}

std::shared_ptr<State> StateMachineComponent::get_state(const std::string& name) const
{
    auto found = _states.find(name);
    if (found != _states.end()) {
        return found->second;
    }
    else {
        return nullptr;
    }
}

void StateMachineComponent::remove_state(const std::string& name)
{
    _states.erase(name);
}

void StateMachineComponent::change_state(const std::string& name)
{
    auto found = _states.find(name);
    if (found != _states.end()) {
        _current_state = found->second;
        _current_state_name = name;
    }
}

DecisionTreeComponent::DecisionTreeComponent(Entity* p, std::shared_ptr<DecisionTreeNode> decision_tree)
    : _decision_tree(decision_tree), Component(p) { }

void DecisionTreeComponent::update(const float& dt) {
    _decision_tree->make_decision(_parent);
}