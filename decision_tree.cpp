#include "decision_tree.hpp"
#include "ai_cmps.hpp"
#include <random>
#include <chrono>

std::shared_ptr<DecisionTreeNode> RandomDecision::get_branch(Entity* owner) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    static std::uniform_int_distribution<int> dist(0, 1);
    bool choice = dist(e) == 0;
    if (choice)
        return _true_node;
    else
        return _false_node;
}

std::shared_ptr<DecisionTreeNode> RandomMultiDecision::get_branch(Entity* owner)
{
    static std::random_device rd;
    static std::default_random_engine e(rd());
    static std::uniform_int_distribution<size_t> dist(0, _child_nodes.size());
    return _child_nodes[dist(e)];
}

std::shared_ptr<DecisionTreeNode> DistanceDecision::get_branch(Entity* owner)
{
    auto length = [](const sf::Vector2f& v) -> float {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    float dist = length(owner->get_position() - _player->get_position());
    if (dist < _distance)
        return _true_node;
    else
        return _false_node;
}

void SteeringDecision::make_decision(Entity* owner)
{
    std::shared_ptr<StateMachineComponent> sm = owner->get_components<StateMachineComponent>()[0];
    sm->change_state(_state);
}