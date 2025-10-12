#include "level_system.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
int LevelSystem::_width;
int LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tile_size(100.f);
vector<unique_ptr<RectangleShape>> LevelSystem::_sprites;

map<LevelSystem::Tile, Color> LevelSystem::_colors{ {WALL, Color::White}, {END, Color::Red} };

sf::Vector2f LevelSystem::_start_position;

void LevelSystem::build_sprites() {
    _sprites.clear();
    for (int y = 0; y < LevelSystem::get_height(); ++y) {
        for (int x = 0; x < LevelSystem::get_width(); ++x) {
            unique_ptr<RectangleShape> s = make_unique<RectangleShape>();
            s->setPosition(get_tile_position({ x, y }));
            s->setSize(Vector2f(_tile_size, _tile_size));
            s->setFillColor(get_color(get_tile({ x, y })));
            _sprites.push_back(move(s));
        }
    }
}

void LevelSystem::load_level(const string& path, float tile_size) {
    _tile_size = tile_size;
    int w = 0, h = 0;
    string buffer;

    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else {
        throw string("Couldn't open level file: ") + path;
    }
    int col = 0;

    Vector2f tile_pos = {0,0};

    vector<Tile> temp_tiles;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        switch (c) {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 's':
            temp_tiles.push_back(START);
            tile_pos = get_tile_position({ col,h });
            _start_position = { tile_pos.x - _tile_size/2, tile_pos.y + _tile_size/2 };
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case '+':
            temp_tiles.push_back(WAYPOINT);
            break;
        case 'n':
            temp_tiles.push_back(ENEMY);
            break;
        case '\n':      // end of line
            if (w == 0) { // if we haven't written width yet
                w = i;      // set width
            }
            col = 0;
            h++; // increment height
            break;
        default:
            cout << c << endl; // Don't know what this tile type is
        }
        col++;
    }
    std::cout << "Tiles read: " << temp_tiles.size() << ", Expected: " << (w * h) << " (" << w << "x" << h << ")" << std::endl;
    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file") + path;
    }
    _tiles = make_unique<Tile[]>(w * h);
    _width = w; //set static class vars
    _height = h;
    copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << endl;
    build_sprites();
}

void LevelSystem::render(RenderWindow& window) {
    for (size_t i = 0; i < _width * _height; ++i) {
        window.draw(*_sprites[i]);
    }
}

Color LevelSystem::get_color(LevelSystem::Tile t) {
    auto it = _colors.find(t);
    if (it == _colors.end()) {
        _colors[t] = Color::Transparent;
    }
    return _colors[t];
}

void LevelSystem::set_color(LevelSystem::Tile t, Color c) {
    _colors[t] = c;
}

LevelSystem::Tile LevelSystem::get_tile(Vector2i p) {
    if (p.x > _width || p.y > _height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

Vector2f LevelSystem::get_tile_position(Vector2i p) {
    return (Vector2f(p.x, p.y) * _tile_size);
}

LevelSystem::Tile LevelSystem::get_tile_at(Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw string("Tile out of range ");
    }
    return get_tile(Vector2i((v - _offset) / (_tile_size)));
}

int LevelSystem::get_height() { return _height; }
int LevelSystem::get_width() { return _width; }

Vector2f LevelSystem::get_start_position() {
    return _start_position;
}
