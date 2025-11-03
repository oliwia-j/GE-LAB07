#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "scenes.hpp"

using param = Parameters;

int main(){
  Scenes::menu = std::make_shared<MenuScene>();
  Scenes::menu->load();
  Scenes::game = std::make_shared<GameScene>();
  Scenes::game->load();
  GameSystem::set_active_scene(Scenes::menu);
  GameSystem::start(param::game_width,param::game_height,"pacman",param::time_step);
  return 0;
}
