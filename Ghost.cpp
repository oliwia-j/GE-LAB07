#include "Ghost.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"
#include "Renderer.hpp"

using namespace sf;
using namespace std;

using param = Parameters;
using ls = LevelSystem;

Ghost::Ghost()
    : Entity(make_unique<CircleShape>(_radius)) {
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(sf::Vector2f(_radius, _radius));
}

void Ghost::update(const float& dt) {
    Entity::update(dt);
}

void Ghost::render() const {
    Renderer::queue(_shape.get());
}
