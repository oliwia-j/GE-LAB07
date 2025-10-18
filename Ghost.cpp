#include "Ghost.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"

using namespace sf;
using namespace std;

using param = Parameters;
using ls = LevelSystem;

Ghost::Ghost() {}

void Ghost::update(const float& dt) {
    Entity::update(dt);
}

void Ghost::render() {
    Entity::render();
}
