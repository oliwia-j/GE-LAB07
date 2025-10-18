#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Parameters {
    static constexpr int game_width = 800;
    static constexpr int game_height = 800;
    static constexpr float entity_size = 25.f;

    static constexpr int ghost_count = 4;
    static constexpr int ghost_speed = 100;

    static constexpr int player_speed = 100;

    static constexpr float time_step = 0.017f; // 60 fps

    static constexpr sf::Keyboard::Key controls[4] = {
        sf::Keyboard::Left,
        sf::Keyboard::Right,
        sf::Keyboard::Up,
        sf::Keyboard::Down,
    };

    static constexpr char* maze_1 = "./Debug/res/levels/maze.txt";
    static constexpr char* maze_2 = "./Debug/res/levels/maze_2.txt";
    static constexpr char* maze_3 = "./Debug/res/levels/maze_3.txt";
};