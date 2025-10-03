#pragma once //insure that this header file is included only once and there will no multiple definition of the same thing
#include <SFML/Graphics.hpp>

struct Parameters {
    static constexpr int game_width = 800;
    static constexpr int game_height = 600;
    static constexpr int sprite_size = 32;
    static constexpr int invaders_cols = 7;
    static constexpr int invaders_rows = 3;
    static constexpr int invaders_step = 20;
    static constexpr int invaders_speedup = 10;
    static constexpr int p_speed = 100;

    static constexpr float time_step = 0.017f; // 60 fps /* moved from main */

    static constexpr sf::Keyboard::Key controls[2] = {
        sf::Keyboard::Left,
        sf::Keyboard::Right
    };
};