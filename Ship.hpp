#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
public:
    Ship();
    // Copy constructor
    Ship(const Ship& s);
    // Constructor that takes a sprite
    Ship(sf::IntRect ir);
    // Pure virtual deconstructor - makes this an abstract class and avoids undefined behaviour!
    virtual ~Ship() = 0;
    virtual void move_down();
    // Update, virtual so can be overridden, but not pure virtual
    virtual void update(const float& dt);
protected:
    sf::IntRect _sprite;
};

class Invader : public Ship {
public:
    static bool direction;
    static float speed;
    Invader();
    Invader(const Invader& inv);
    Invader(sf::IntRect ir, sf::Vector2f pos);
    void move_down() override;
    void update(const float& dt) override;
};

class Player : public Ship {
public:
    Player();
    void update(const float& dt) override;
};