#pragma once
#include "game_system.hpp"

class MenuScene : public Scene {
public:
    MenuScene() = default;
    void update(const float& dt) override;
    void render() override;
    void load()override;
    void unload() override;
private:
    std::shared_ptr<Entity> _player;
    sf::Text _text;
    sf::Font _font;
};


class LevelScene : public Scene {
public:
    LevelScene() = default;
    void update(const float& dt) override;
    void render() override;
    void load()override;
    void unload() override;
private:
    std::shared_ptr<Entity> _player;
    std::vector<std::shared_ptr<Entity>> _walls;
    void _load_level(const std::string& file_path);
};

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> level;
};