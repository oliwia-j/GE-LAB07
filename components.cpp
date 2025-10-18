#include "components.hpp"
#include "game_parameters.hpp"
#include "Renderer.hpp"
#include <SFML/Graphics.hpp>

using param = Parameters;

void ShapeComponent::update(const float& dt) {
	_shape->setPosition(_parent->get_position());
}

void ShapeComponent::render() { 
	Renderer::queue(_shape.get()); 
}

sf::Shape& ShapeComponent::get_shape() const { 
	return *_shape; 
}

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}

/////

void ActorMovementComponent::update(const float& dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
	: _speed(100.0f), Component(p) {}

//bool ActorMovementComponent::_valid_move(const sf::Vector2f& pos) {
//	return (ls::get_tile_at(pos) != ls::WALL);
//}

void ActorMovementComponent::move(const sf::Vector2f& p) {
	sf::Vector2f new_pos = _parent->get_position() + p;
	//if (_valid_move(new_pos)) {
	//	_parent->set_position(new_pos);
	//}
}

void ActorMovementComponent::move(float x, float y) {
	move(sf::Vector2f(x, y));
}
float ActorMovementComponent::get_speed() const { 
    return _speed;
}
void ActorMovementComponent::set_speed(float speed) { 
    _speed = speed;
}

/////

PlayerMovementComponent::PlayerMovementComponent(Entity* p)
	: ActorMovementComponent(p) {}

void PlayerMovementComponent::update(const float& dt) {
    // Move in four directions based on keys
    sf::Vector2f movement(0.f, 0.f);
    sf::Vector2f new_pos(0.f, 0.f);
    bool in_bounds = true;

    // Move Left
    if (sf::Keyboard::isKeyPressed(param::controls[0])) {
        movement = sf::Vector2f({ dt * -1 * _speed, 0.0f });
        new_pos = _parent->get_position() + movement;
        in_bounds = _parent->get_position().x > param::entity_size;
        std::cout << "move left" << std::endl;
    }
    // Move Right
    if (sf::Keyboard::isKeyPressed(param::controls[1])) {
        movement = sf::Vector2f({ dt * _speed, 0.0f });
        new_pos = _parent->get_position() + movement;
        in_bounds = _parent->get_position().x < param::game_width - param::entity_size;
        std::cout << "move right" << std::endl;
    }
    // Move Up
    if (sf::Keyboard::isKeyPressed(param::controls[2])) {
        movement = sf::Vector2f({ 0.0f, dt * -1 * _speed });
        new_pos = _parent->get_position() + movement;
        in_bounds = _parent->get_position().y > param::entity_size;
        std::cout << "move up" << std::endl;
    }
    // Move Down
    if (sf::Keyboard::isKeyPressed(param::controls[3])) {
        movement = sf::Vector2f({ 0.0f, dt * _speed });
        new_pos = _parent->get_position() + movement;
        in_bounds = _parent->get_position().y < param::game_height - param::entity_size;
        std::cout << "move down" << std::endl;
    }

    //if (in_bounds && valid_move(new_pos)) {
    //    move(movement);
    //}
}

/////

EnemyAIComponent::EnemyAIComponent(Entity* p)
    : ActorMovementComponent(p) {}

void EnemyAIComponent::update(const float& dt) {}