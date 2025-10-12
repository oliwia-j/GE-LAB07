#include "game_parameters.hpp"
#include "game_system.hpp"
#include "Scenes.hpp"

using param = Parameters;

int main() {
	Scenes::maze = std::make_shared<MazeScene>();
	std::static_pointer_cast<MazeScene>(Scenes::maze)->set_file_path(param::maze_1);
	Scenes::maze->load();
	Scenes::end = std::make_shared<EndScene>();
	Scenes::end->load();
	GameSystem::set_active_scene(Scenes::maze);
	GameSystem::start(param::game_width, param::game_height, "GE-LAB", param::time_step);
	return 0;
}