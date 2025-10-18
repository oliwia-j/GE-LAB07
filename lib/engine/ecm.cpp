#include "ecm.hpp"
#include <iostream>
#include "Renderer.hpp"

Entity::Entity() {}

Entity::~Entity() {}

//const sf::Vector2f Entity::get_position() { return _position; }

void Entity::update(const float& dt) {
	for (std::shared_ptr<Component>& c : _components) {
		c->update(dt);
	}
}

void Entity::render() {
	for (std::shared_ptr<Component>& c : _components) {
		c->render();
	}
}

const sf::Vector2f& Entity::get_position() const {
	return _position;
}

void Entity::set_position(const sf::Vector2f& pos) {
	_position = pos;
	//_shape->setPosition(sf::Vector2f(_position));
}

bool Entity::is_alive() const {
	return _alive;
}

void Entity::set_alive(bool alive) {
	_alive = alive;
}

Component::Component(Entity* const p) : _parent(p) {};

void EntityManager::update(double dt) {
	for (std::shared_ptr<Entity>& item : list) {
		if (item->is_alive()) {
			item->update(dt);
		}
	}
}
void EntityManager::render() {
	for (std::shared_ptr<Entity>& item : list) {
		item->render();
	}
}