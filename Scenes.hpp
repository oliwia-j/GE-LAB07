#pragma once
#include <string>
#include "game_system.hpp"

struct Scenes {
	static std::shared_ptr<Scene> maze;
};

class MazeScene : public Scene {
public:
	MazeScene() = default;
	void update(const float& dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
	void reset();
	void set_file_path(const std::string& file_path);
private:
	std::string _file_path;
};