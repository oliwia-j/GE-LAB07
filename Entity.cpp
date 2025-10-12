#include "entity.hpp"
#include <iostream>
Entity::Entity(std::unique_ptr<sf::Shape> s) : _shape(std::move(s)) {}

const sf::Vector2f Entity::get_position() { return _position; }

void Entity::set_position(const sf::Vector2f& pos) {
	_position = pos;
	_shape->setPosition(sf::Vector2f(_position));
}

void Entity::move(const sf::Vector2f& pos) { 
	_position += pos;
	_shape->setPosition(_position);
}

void Entity::update(const float& dt) {}