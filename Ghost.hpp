#pragma once
#include "ecm.hpp"
#include <iostream>

class Ghost : public Entity {
public:
	Ghost();
	void update(const float& dt) override;
	void render() override;

private:
	static constexpr float _radius = 25.f;
	static constexpr float _speed = 200.f;
};