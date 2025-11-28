#pragma once
#include "ai_cmps.hpp"

class NormalState : public State
{
private:
    std::shared_ptr<Entity> _player;
public:
    NormalState(std::shared_ptr<Entity> player) : _player(player) { }
    void execute(Entity*, const float&) override;
};

class NearState : public State
{
private:
    std::shared_ptr<Entity> _player;
public:
    NearState(std::shared_ptr<Entity> player) : _player(player) { }
    void execute(Entity*, const float&) override;
};

class SteeringState : public State
{
private:
    SteeringFunction _steering;
    std::shared_ptr<Entity> _player;
    float _max_speed;
    sf::Color _color;
public:
    SteeringState(SteeringFunction steering, std::shared_ptr<Entity> player, float max_speed, sf::Color color);
    void execute(Entity*, const float&) override;
};