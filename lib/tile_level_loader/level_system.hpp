#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

class LevelSystem {
public:
    enum Tile { EMPTY, START, END, WALL, ENEMY, WAYPOINT };

    static void load_level(const std::string&, float tile_size = 50.f);
    static void render(sf::RenderWindow& window);
    static sf::Color get_color(Tile t);
    static void set_color(Tile t, sf::Color c);
    //Get Tile at grid coordinate
    static Tile get_tile(sf::Vector2i);
    //Get Screenspace coordinate of tile
    static sf::Vector2f get_tile_position(sf::Vector2i);
    //get the tile at screenspace pos
    static Tile get_tile_at(sf::Vector2f);
    static std::vector<sf::Vector2i> find_tiles(Tile);
    static int get_height();
    static int get_width();
    static sf::Vector2f get_start_position();

protected:
    static std::unique_ptr<Tile[]> _tiles; //Internal array of tiles
    static int _width; //how many tiles wide is level
    static int _height; //how many tile high is level
    static sf::Vector2f _offset; //Screenspace offset of level, when rendered.
    static float _tile_size; //Screenspace size of each tile, when rendered.
    static std::map<Tile, sf::Color> _colors; //color to render each tile type
    static sf::Vector2f _start_position; //starting position for the player

    //array of sfml sprites of each tile
    static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;
    //generate the _sprites array
    static void build_sprites();

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};