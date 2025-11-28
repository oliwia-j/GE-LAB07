#include <random>
#include "scenes.hpp"
#include "renderer.hpp"
#include "graphics_cmps.hpp"
#include "ctrl_cmps.hpp"
#include "ai_cmps.hpp"
#include "game_parameters.hpp"


using gs = GameSystem;
using param = Parameters;

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::steering;

void MenuScene::update(const float& dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        gs::set_active_scene(Scenes::steering);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {

    }
    Scene::update(dt);
}

void MenuScene::render() {
    Renderer::queue(&_text);
    Scene::render();
}

void MenuScene::load() {
    _font.loadFromFile("resources/fonts/RobotoMono-Regular.ttf");
    _text.setFont(_font);
    _text.setCharacterSize(60);
    _text.setString("Movement Demos\nPress 1 for Steering\nPress 2 for Pathfinding");
}

void MenuScene::unload() {}


void SteeringScene::load() {
    std::shared_ptr<Entity> player = make_entity();
    player->set_position(sf::Vector2f(param::game_width / 2,
        param::game_height / 2));
    std::shared_ptr<ShapeComponent> s = player->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Red);
    player->add_component<KeyboardMovementComponent>();

    // Setup C++ random number generation
    std::random_device dev;
    std::default_random_engine engine(dev());
    std::uniform_real_distribution<float> x_dist(0.0f,
        param::game_width);
    std::uniform_real_distribution<float> y_dist(0.0f,
        param::game_height);
    for (size_t n = 0; n < 100; ++n) {
        std::shared_ptr<Entity> enemy = make_entity();
        enemy->set_position(sf::Vector2f(x_dist(engine), y_dist(engine)));
        std::shared_ptr<ShapeComponent> s = enemy->add_component<ShapeComponent>();
        s->set_shape<sf::RectangleShape>(sf::Vector2f(10.0f, 10.0f));
        s->get_shape().setFillColor(sf::Color::Blue);
        enemy->add_component<SteeringComponent>(player.get(), 50.f);
    }
}

void SteeringScene::unload() {
    Scene::unload();
}

void SteeringScene::update(const float& dt) {
    Scene::update(dt);
}

void SteeringScene::render() {
    Scene::render();
}