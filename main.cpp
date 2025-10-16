#include "game_parameters.hpp"
#include "game_system.hpp"
#include "Scenes.hpp"

using param = Parameters;

int main() {
	std::cout << "main start" << std::endl;
	Scenes::menu = std::make_shared<MenuScene>();
	Scenes::menu->load();
	Scenes::game = std::make_shared<GameScene>();
	Scenes::game->load();
	GameSystem::set_active_scene(Scenes::menu);
	GameSystem::start(param::game_width, param::game_height, "GE-LAB", param::time_step);
	return 0;
}