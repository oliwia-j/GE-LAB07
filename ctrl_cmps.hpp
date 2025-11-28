#pragma once
#include "ecm.hpp"

// A component to allow basic movement behaviour
class KeyboardMovementComponent : public Component {
protected:
	// Speed we can travel
	float _speed;
	// Checks if the move is valid.
	bool valid_move(const sf::Vector2f&);

public:
	// Will check the keyboard and move the component's parent.
	void update(const float&) override;
	// Moves the component's parent.
	void move(const sf::Vector2f&);
	// Moves the component's parent.
	void move(float x, float y);
	// Component does not need rendered.
	void render() override {}
	// Used to create the component for an entity
	explicit KeyboardMovementComponent(Entity* p);

	KeyboardMovementComponent() = delete;
};