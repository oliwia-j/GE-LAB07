#include "game_parameters.hpp"
#include "scenes.hpp"

using param = Parameters;

int main() {
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::menu->load();
    Scenes::steering = std::make_shared<SteeringScene>();
    Scenes::steering->load();
    Scenes::pathfinding = std::make_shared<PathfindingScene>();
    Scenes::pathfinding->load();
    Scenes::state = std::make_shared<StateScene>();
    Scenes::state->load();
    Scenes::decision = std::make_shared<DecisionScene>();
    Scenes::decision->load();
    GameSystem::set_active_scene(Scenes::menu);
    GameSystem::start(param::game_width, param::game_height, "AI Decision", param::time_step, false);
    return 0;
}