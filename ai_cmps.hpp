#pragma once
#include <SFML/Graphics.hpp>
#include <ecm.hpp>


// Output from a steering behaviour.
struct SteeringOutput
{
    // Direction of travel.
    sf::Vector2f direction;
    // Rotation of travel.
    float rotation;
};


struct SteeringBehaviours {
    static SteeringOutput seek(const sf::Vector2f&, const sf::Vector2f&);
    static SteeringOutput flee(const sf::Vector2f&, const sf::Vector2f&);
};

class SteeringComponent : public Component {
protected:
    Entity* _player;
    float _max_speed;
    bool valid_move(const sf::Vector2f&) const;
public:
    void update(const float&) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity* p, Entity* player, float _max_speed);
    SteeringComponent() = delete;
};