#include <random>
#include "scenes.hpp"
#include "renderer.hpp"
#include "graphics_cmps.hpp"
#include "ctrl_cmps.hpp"
#include "ai_cmps.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"
#include "astar.hpp"
#include "states.hpp"

using gs = GameSystem;
using param = Parameters;
using ls = LevelSystem;

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::steering;
std::shared_ptr<Scene> Scenes::pathfinding;
std::shared_ptr<Scene> Scenes::state;
std::shared_ptr<Scene> Scenes::decision;

void MenuScene::update(const float& dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        gs::set_active_scene(Scenes::steering);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        gs::set_active_scene(Scenes::pathfinding);
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
    _text.setString("Movement Demos\nPress 1 for Steering\nPress 2 for Pathfinding\n");
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

void PathfindingScene::load() {
    ls::load_level("./resources/levels/pacman.txt", 20.0f);

    _character = make_entity();
    sf::Vector2f start_pos(ls::get_start_position() - sf::Vector2f({10.f, 10.f}));
    _character->set_position(start_pos);
    std::shared_ptr<ShapeComponent> s = _character->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Red);

    std::vector<sf::Vector2i> path = a_star::path_find(sf::Vector2i(1, 1),
        sf::Vector2i(ls::get_width() - 2, ls::get_height() - 2));
    std::shared_ptr<PathfindingComponent> cmp = _character->add_component<PathfindingComponent>();
    cmp->set_path(path);
}

void PathfindingScene::unload() { Scene::unload(); }

void PathfindingScene::update(const float& dt) {
    static bool mouse_down = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouse_down) {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(Renderer::get_window());
        mouse_down = true;
        if (mouse_pos.x > 0 && mouse_pos.x < param::game_width &&
            mouse_pos.y > 0 && mouse_pos.y < param::game_height) {
            // sf::Vector2f relative_pos = mouse_pos; - sf::Vector2i(ls::getOffset());
            sf::Vector2i tile_coord = mouse_pos / static_cast<int>(param::tile_size);
            if (ls::get_tile(sf::Vector2<size_t>(tile_coord)) != ls::WALL) {
                sf::Vector2f char_pos = _character->get_position();
                sf::Vector2i char_tile = sf::Vector2i(char_pos / param::tile_size);
                std::vector<sf::Vector2i> path = a_star::path_find(char_tile, tile_coord);
                //_pathfinder->set_path(path);
            }
        }
    }
    if (mouse_down && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouse_down = false;
    }
    Scene::update(dt);
}

void PathfindingScene::render() {
    ls::render(Renderer::get_window());
    Scene::render();
}

void StateScene::load() {
    std::shared_ptr<Entity> player = make_entity();
    player->set_position(sf::Vector2f(param::game_width / 2.f, param::game_height / 2.f));
    auto s = player->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Red);
    player->add_component<KeyboardMovementComponent>();

    std::shared_ptr<Entity> enemy = make_entity();
    enemy->set_position(sf::Vector2f(param::game_width / 4.f, param::game_height / 4.f));
    s = enemy->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::Blue);

    std::shared_ptr<StateMachineComponent> sm = enemy->add_component<StateMachineComponent>();
    sm->add_state("normal", std::make_shared<NormalState>(player));
    sm->add_state("near", std::make_shared<NearState>(player));
    sm->change_state("normal");
}

void StateScene::unload() {
    Scene::unload();
}

void StateScene::update(const float& dt) {
    Scene::update(dt);
}

void StateScene::render() {
    Scene::render();
}

void DecisionScene::load() {
    std::shared_ptr<Entity> player = make_entity();
    player->set_position(sf::Vector2f(param::game_width / 2,
        param::game_height / 2));
    std::shared_ptr<ShapeComponent> s = player->add_component<ShapeComponent>();
    s->set_shape<sf::CircleShape>(10.0f);
    s->get_shape().setFillColor(sf::Color::White);
    player->add_component<KeyboardMovementComponent>();

    std::random_device dev;
    std::default_random_engine engine(dev());
    std::uniform_real_distribution<float> x_dist(0.0f, param::game_width);
    std::uniform_real_distribution<float> y_dist(0.0f, param::game_height);

    for (size_t n = 0; n < 100; ++n)
    {
        std::shared_ptr<Entity> enemy = make_entity();
        enemy->set_position(sf::Vector2f(x_dist(engine), y_dist(engine)));
        std::shared_ptr<ShapeComponent> s = enemy->add_component<ShapeComponent>();
        s->set_shape<sf::CircleShape>(10.0f);
        s->get_shape().setFillColor(sf::Color::Blue);

        auto sm = enemy->add_component<StateMachineComponent>();
        sm->add_state("stationary", std::make_shared<SteeringState>(SteeringBehaviours::stationary, player, 50.f, sf::Color::Blue));
        sm->add_state("seek", std::make_shared<SteeringState>(SteeringBehaviours::seek, player, 50.f, sf::Color::Green));
        sm->add_state("flee", std::make_shared<SteeringState>(SteeringBehaviours::flee, player, 50.f, sf::Color::Yellow));

        auto decision = std::make_shared<DistanceDecision>(
            player,
            50.0f,
            std::make_shared<SteeringDecision>("flee"),
            std::make_shared<DistanceDecision>(
                player,
                100.0f,
                std::make_shared<RandomDecision>(
                    std::make_shared<SteeringDecision>("seek"),
                    std::make_shared<SteeringDecision>("stationary")),
                std::make_shared<SteeringDecision>("seek")));

        enemy->add_component<DecisionTreeComponent>(decision);
    }
}


void DecisionScene::unload() { Scene::unload(); }

void DecisionScene::update(const float& dt) {
    Scene::update(dt);
}

void DecisionScene::render() {
    Scene::render();
}