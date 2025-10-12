#pragma once
#include "Entity.hpp"
#include <iostream>

class Player : public Entity {
public:
	Player();
	void update(const float& dt) override;
	void render(sf::RenderWindow& window) const override;
	bool valid_move(sf::Vector2f pos);

private:
	static constexpr float _radius = 25.f;
	static constexpr float _speed = 200.f;
};