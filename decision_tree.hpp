#pragma once
#include "ecm.hpp"

class DecisionTreeNode
{
public:
    virtual ~DecisionTreeNode() = default;
    virtual void make_decision(Entity* owner) = 0;
};

class BinaryDecision : public DecisionTreeNode
{
protected:
    std::shared_ptr<DecisionTreeNode> _true_node;
    std::shared_ptr<DecisionTreeNode> _false_node;
    virtual std::shared_ptr<DecisionTreeNode> get_branch(Entity* owner) = 0;
public:
    BinaryDecision(std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node)
        : _true_node(true_node), _false_node(false_node) { }

    void make_decision(Entity* owner) {
        get_branch(owner)->make_decision(owner);
    }
};

class MultiDecision : public DecisionTreeNode
{
protected:
    std::vector<std::shared_ptr<DecisionTreeNode>> _child_nodes;
    virtual std::shared_ptr<DecisionTreeNode> get_branch(Entity* owner) = 0;
public:
    MultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>>& child_nodes) : _child_nodes(child_nodes) { }

    void make_decision(Entity* owner) {
        get_branch(owner)->make_decision(owner);
    }
};

class RandomDecision : public BinaryDecision
{
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity* owner);
public:
    RandomDecision(std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node) : BinaryDecision(true_node, false_node) { }
};

class RandomMultiDecision : public MultiDecision
{
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity* owner) override;
public:
    RandomMultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>>& child_nodes) : MultiDecision(child_nodes) { }
};

class DistanceDecision : public BinaryDecision
{
private:
    std::shared_ptr<Entity> _player;
    float _distance;
protected:
    std::shared_ptr<DecisionTreeNode> get_branch(Entity* owner);
public:
    DistanceDecision(std::shared_ptr<Entity> player, float distance, std::shared_ptr<DecisionTreeNode> true_node, std::shared_ptr<DecisionTreeNode> false_node)
        : _player(player), _distance(distance), BinaryDecision(true_node, false_node) { }
};

class SteeringDecision : public DecisionTreeNode
{
public:
    SteeringDecision(const std::string& state) : _state(state) {}
    void make_decision(Entity* owner);
private:
    std::string _state;
};