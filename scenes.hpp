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
    sf::Text _text;
    sf::Font _font;
};


class SteeringScene : public Scene {
public:
    SteeringScene() = default;
    void update(const float& dt) override;
    void render() override;
    void load()override;
    void unload() override;
};

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> steering;
};