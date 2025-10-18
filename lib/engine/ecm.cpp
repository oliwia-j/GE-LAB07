#include "ecm.hpp"

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

EntityManager::EntityManager() {};

void EntityManager::set_player(std::shared_ptr<Entity> p) {
	_player = p;
};

std::shared_ptr<Entity> EntityManager::get_player() {
	return _player;
};

void EntityManager::add_enemy(std::shared_ptr<Entity> e) {
	_enemies.push_back(e);
};

std::vector<std::shared_ptr<Entity>> EntityManager::get_enemies() {
	return _enemies;
};

void EntityManager::update(double dt) {
	if (_player && _player->is_alive()) {
		_player->update(dt);
	}
	for (std::shared_ptr<Entity>& item : _enemies) {
		if (item->is_alive()) {
			item->update(dt);
		}
	}
}
void EntityManager::render() {
	if (_player && _player->is_alive()) {
		_player->render();
	}
	for (std::shared_ptr<Entity>& item : _enemies) {
		item->render();
	}
}