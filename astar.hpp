#pragma once

#include "game_system.hpp"
#include <vector>

namespace a_star {
	std::vector<sf::Vector2i> path_find(sf::Vector2i start, sf::Vector2i finish);
}
class Node {
private:
	sf::Vector2i _pos;
	int _level;
	int _priority;

public:
	Node() = default;
	Node(const sf::Vector2i& pos, int level, int priority);

	const sf::Vector2i& get_pos() const;

	int get_level() const;

	int get_priority() const;

	unsigned int estimate(const sf::Vector2i& dest) const;

	void update_priority(const sf::Vector2i& dest);

	void next_level();

	// Used for priority ordering
	bool operator<(const Node& other) const;
};