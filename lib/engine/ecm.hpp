#pragma once
#include <iostream>
#include <algorithm>
#include <memory>
#include <typeindex>
#include <vector>
#include <SFML/Graphics.hpp>


class Component; //forward declare

class Entity {
public:
	Entity();
	virtual ~Entity();
	virtual void update(const float& dt);
	virtual void render();

	const sf::Vector2f& get_position() const;
	void set_position(const sf::Vector2f& _position);
	//bool is_for_deletion() const;
	//float get_rotation() const;
	//void set_rotation(float _rotation);
	bool is_alive() const;
	void set_alive(bool alive);
	//void set_for_delete();
	//bool is_visible() const;
	//void set_visible(bool _visible);

	template <typename T, typename... Targs>
	std::shared_ptr<T> add_component(Targs... params) {
		static_assert(std::is_base_of<Component, T>::value, "T != component");
		std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
		_components.push_back(sp);
		return sp;
	}

	// Will return a T component, or anything derived from a T component.
	template <typename T>
	const std::vector<std::shared_ptr<T>> get_compatible_components() {
		static_assert(std::is_base_of<Component, T>::value, "T != component");
		std::vector<std::shared_ptr<T>> ret;
		for (auto c : _components) {
			auto dd = dynamic_cast<T*>(&(*c));
			if (dd) {
				ret.push_back(std::dynamic_pointer_cast<T>(c));
			}
		}
		return ret;
	}

protected:
	std::vector<std::shared_ptr<Component>> _components;
	sf::Vector2f _position;
	//float _rotation;
	bool _alive;       // should be updated
	//bool _visible;     // should be rendered
	//bool _for_deletion; // should be deleted
};

class Component {
public:
	Component() = delete;
	//bool is_for_deletion() const;
	virtual void update(const float& dt) = 0;
	virtual void render() = 0;
	//virtual ~Component();
protected:
	Entity* const _parent;
	//bool _for_deletion; // should be removed
	explicit Component(Entity* const p);
};

class EntityManager {
public:
	EntityManager();
	~EntityManager() = default;
	void update(double dt);
	void render();
	void set_player(std::shared_ptr<Entity> p);
	std::shared_ptr<Entity> get_player();
	void add_enemy(std::shared_ptr<Entity> e);
	std::vector<std::shared_ptr<Entity>> get_enemies();
protected:
	std::vector<std::shared_ptr<Entity>> _enemies;
	std::shared_ptr<Entity> _player;
};