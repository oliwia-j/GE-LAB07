#include "astar.hpp"
#include <cmath>
#include <array>
#include <queue>
#include "level_system.hpp"

using ls = LevelSystem;

std::vector<sf::Vector2i> a_star::path_find(sf::Vector2i start, sf::Vector2i finish) {
    static std::array<sf::Vector2i, 4> directions = {
        sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, -1) };

    // This may not be the most efficient mechanism, but should not be a // problem unless your maps get very big.
    std::vector<std::vector<bool>> closed_nodes_map(ls::get_width());
    std::vector<std::vector<int>> open_nodes_map(ls::get_width());
    std::vector<std::vector<sf::Vector2i>> direction_map(ls::get_width());
    // Queue of nodes to test.  Priority ordered.
    // We need two for when we redirect and copy the path.
    std::priority_queue<Node> queue[2];
    // Index of current queue
    size_t queue_index = 0;

    // Initialise the lookup maps.  Initially everything looks traversable and
    // no route has been planned.
    for (size_t y = 0; y < ls::get_height(); ++y) {
        for (size_t x = 0; x < ls::get_width(); ++x) {
            closed_nodes_map[x].push_back(false);
            open_nodes_map[x].push_back(0);
            direction_map[x].emplace_back(sf::Vector2i(0, 0));
        }
    }

    // Add start node to the queue - we will check it first
    Node n0(start, 0, 0);
    // Update the priority of the node.
    n0.update_priority(finish);
    // Push onto the current queue of nodes to check.
    queue[queue_index].push(n0);
    // And update the value in the open_nodes_map.
    open_nodes_map[start.x][start.y] = n0.get_priority();

    // Loop until queue is empty
    while (!queue[queue_index].empty()) {
        auto tmp = queue[queue_index].top();
        n0 = Node(tmp.get_pos(), tmp.get_level(), tmp.get_priority());
        auto pos = n0.get_pos();

        // Remove node from open list
        queue[queue_index].pop();
        open_nodes_map[pos.x][pos.y] = 0;
        // Mark on closed node list
        closed_nodes_map[pos.x][pos.y] = true;

        // Check if reached goal
        if (pos == finish) {
            std::vector<sf::Vector2i> path;
            while (!(pos == start)) {
                // Get the direction
                auto dir = direction_map[pos.x][pos.y];
                // Add the current position to the path
                path.push_back(pos);
                pos += dir;
            }
            // Path is inverted.  Reverse it.
            std::reverse(std::begin(path), std::end(path));
            // Return path.
            return path;
        }
        // Not reached goal.  Check four directions.
        for (auto& dir : directions) {
            auto next = pos + dir;

            // Check if next is valid.
            if (!(next.x < 0 || next.x > ls::get_width() || next.y < 0 ||
                next.y > ls::get_height() ||
                ls::get_tile(sf::Vector2<size_t>(next.x, next.y)) == ls::WALL ||
                closed_nodes_map[next.x][next.y])) {
                // Generate new node
                Node m0(next, n0.get_level(), n0.get_priority());
                m0.next_level();
                m0.update_priority(finish);

                // Check if new node has no priority
                if (open_nodes_map[next.x][next.y] == 0) {
                    // Update priority and add to the queue.
                    open_nodes_map[next.x][next.y] = m0.get_priority();
                    queue[queue_index].push(m0);
                    // Put the opposite direction into the direction map.
                    // We work backwards.
                    direction_map[next.x][next.y] = dir * -1;
                }
                // If it has a priority, check if the priority is better on the new // route.
                else if (open_nodes_map[next.x][next.y] > m0.get_priority()) {
                    // Update the priority information
                    open_nodes_map[next.x][next.y] = m0.get_priority();
                    // Update the direction map with the inverse direction.
                    direction_map[next.x][next.y] = dir * -1;
                    // Now replace the node via swapping across the lists and ignoring
                    // the old one.
                    while (queue[queue_index].top().get_pos() != next) {
                        queue[1 - queue_index].push(queue[queue_index].top());
                        queue[queue_index].pop();
                    }
                    queue[queue_index].pop();
                    if (queue[queue_index].size() > queue[1 - queue_index].size())
                        queue_index = 1 - queue_index;
                    while (!queue[queue_index].empty()) {
                        queue[1 - queue_index].push(queue[queue_index].top());
                        queue[queue_index].pop();
                    }
                    queue_index = 1 - queue_index;
                    queue[queue_index].push(m0);
                }
            }
        }
    }
    return std::vector<sf::Vector2i>();
}

Node::Node(const sf::Vector2i& pos, int level, int priority) : _pos(pos), _level(level), _priority(priority) {}

const sf::Vector2i& Node::get_pos() const { return _pos; }

int Node::get_level() const { return _level; }

int Node::get_priority() const { return _priority; }

unsigned int Node::estimate(const sf::Vector2i& dest) const {
    auto length = [](sf::Vector2i v) -> int {
        return std::sqrt(v.x * v.x + v.y * v.y);
        };
    sf::Vector2i delta = dest - _pos;
    return static_cast<unsigned int>(length(delta));
}

void Node::update_priority(const sf::Vector2i& dest) {
    // Heuristic is just Euclidian distance.
    // Can be modified.
    _priority = _level + estimate(dest) * 10;
}

void Node::next_level() { _level += 10; }

// Used for priority ordering
bool Node::operator<(const Node& other) const {
    return _priority > other._priority;
}