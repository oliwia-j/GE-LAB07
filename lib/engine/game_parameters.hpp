#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Parameters {
    static constexpr int game_width = 900;
    static constexpr int game_height = 900;
    static constexpr float entity_size = 15.f;
    static constexpr float tile_size = 30.f;

    static constexpr int ghost_count = 4;
    static constexpr int ghost_speed = 100;

    static constexpr int player_speed = 150;

    static constexpr float time_step = 0.017f; // 60 fps

    static constexpr sf::Keyboard::Key controls[4] = {
        sf::Keyboard::Left,
        sf::Keyboard::Right,
        sf::Keyboard::Up,
        sf::Keyboard::Down,
    };

    static constexpr char* font_path = "./Debug/res/fonts/Roboto/static/Roboto-Bold.ttf";
    static constexpr char* pacman_map = "./Debug/res/levels/pacman.txt";
};