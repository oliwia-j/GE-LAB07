#pragma once

struct Parameters{
    static constexpr int game_width = 700;
    static constexpr int game_height = 700;
    static constexpr float time_step = 0.017f;
    static constexpr int number_ghosts = 4;
    static constexpr float player_speed = 150.f;
    static constexpr float ghost_speed = 100.f;
    static constexpr float entity_size = 12.f; 
    static constexpr float tile_size = 25.f;
    static constexpr float small_speed_up = 5.f;
    static constexpr float big_speed_up = 10.f;
    static constexpr float small_nibble_size = 3.f;
    static constexpr float big_nibble_size = 6.f;
    static constexpr char* font_path = "./resources/fonts/RobotoMono-Regular.ttf";
    static constexpr char* pacman_map_path = "./resources/levels/pacman.txt";
};