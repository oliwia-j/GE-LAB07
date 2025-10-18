#pragma once
#include "ecm.hpp"
#include <iostream>
#include "game_parameters.hpp"
#include "level_system.hpp"

class Player : public Entity {
public:
	Player();
	void update(const float& dt) override;
	void render() override;
	bool valid_move(sf::Vector2f pos);

private:
	
	static constexpr float _speed = 200.f;
};