#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "ecm.hpp"
#include "decision_tree.hpp"


// Output from a steering behaviour.
struct SteeringOutput
{
    // Direction of travel.
    sf::Vector2f direction;
    // Rotation of travel.
    float rotation;
};

using SteeringFunction = std::function<SteeringOutput(const sf::Vector2f&, const sf::Vector2f&)>;

struct SteeringBehaviours {
    static SteeringFunction seek;
    static SteeringFunction flee;
    static SteeringFunction stationary;
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

class PathfindingComponent : public Component {
protected:
    std::vector<sf::Vector2i> _path;
    size_t _index = 0;
    double _elapsed = 0.0;

public:
    void update(const float&) override;
    void render() override {}
    void set_path(std::vector<sf::Vector2i>& path);
    explicit PathfindingComponent(Entity* p);
    PathfindingComponent() = delete;
};


class State
{
public:
    virtual ~State() = default;
    virtual void execute(Entity*, const float&) = 0;
};


class StateMachineComponent : public Component
{
private:
    std::unordered_map<std::string, std::shared_ptr<State>> _states;
    std::shared_ptr<State> _current_state;
    std::string _current_state_name = "";
public:
    StateMachineComponent() = delete;
    explicit StateMachineComponent(Entity* p);

    void update(const float&) override;
    void render() override { }
    void add_state(const std::string&, std::shared_ptr<State>);
    std::shared_ptr<State> get_state(const std::string&) const;
    void remove_state(const std::string&);
    void change_state(const std::string&);
    const std::string& current_state() const { return _current_state_name; }
};


class DecisionTreeComponent : public Component
{
private:
    std::shared_ptr<DecisionTreeNode> _decision_tree;
public:
    void update(const float&) override;
    void render() override { }
    explicit DecisionTreeComponent(Entity* p, std::shared_ptr<DecisionTreeNode> decision_tree);
    DecisionTreeComponent() = delete;
};