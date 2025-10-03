#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "ship.hpp"

struct GameSystem {
    // Global variables
    static sf::Texture spritesheet;
    static std::vector<std::shared_ptr<Ship>> ships; // Vector of shared pointers to Ships

    // Game system functions
    static void init();
    static void clean();
    static void update(const float& dt);
    static void render(sf::RenderWindow& window);
};