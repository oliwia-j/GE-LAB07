#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "game_system.hpp"
#include "Player.hpp"
#include "Ghost.hpp"
#include "Renderer.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "level_system.hpp"
#include "components.hpp"

struct Scenes {
	static std::shared_ptr<Scene> menu;
	static std::shared_ptr<Scene> game;
};

class MenuScene : public Scene {
private:
	sf::Text text;
	sf::Font font;

public:
	MenuScene() = default;
	void update(const float& dt) override;
	void render() override;
	void load()override;
};

class GameScene : public Scene {
private:
	sf::Text text;
	sf::Clock scoreClock;

	void respawn();

public:
	GameScene() = default;
	void update(const float& dt) override;
	void render() override;
	void load() override;
};