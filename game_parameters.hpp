#pragma once
#include <SFML/Graphics.hpp>

struct Parameters {
    static constexpr int game_width = 800;
    static constexpr int game_height = 600;
    static constexpr int sprite_size = 32;

    static constexpr float time_step = 0.017f; // 60 fps

    static constexpr sf::Keyboard::Key controls[4] = {
        sf::Keyboard::Left,
        sf::Keyboard::Right,
        sf::Keyboard::Up,
        sf::Keyboard::Down,
    };
};