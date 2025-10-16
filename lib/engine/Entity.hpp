#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
public:
	Entity(std::unique_ptr<sf::Shape> s);
	Entity() = delete;
	virtual ~Entity() = default;

	virtual void update(const float& dt);
	virtual void render() const = 0;

	const sf::Vector2f get_position();
	void set_position(const sf::Vector2f& pos);
	void move(const sf::Vector2f& pos);

protected:
	std::unique_ptr<sf::Shape> _shape;
	sf::Vector2f _position;
};

struct EntityManager {
	std::vector<std::shared_ptr<Entity>> list;
	void update(double dt);
	void render();
};