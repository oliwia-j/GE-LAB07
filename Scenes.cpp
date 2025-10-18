#include "Scenes.hpp"
#include "Player.hpp"
#include "Ghost.hpp"
#include "Renderer.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "level_system.hpp"
#include "components.hpp"

using ls = LevelSystem;
using param = Parameters;

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::game;

void MenuScene::update(const float& dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        GameSystem::set_active_scene(Scenes::game);
    }
    Scene::update(dt);
}

void MenuScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

void MenuScene::load() {
    em = std::make_shared<EntityManager>();
    font.loadFromFile("./Debug/res/fonts/Roboto/static/Roboto-Bold.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setString("Almost Pacman!");
    text.setPosition((param::game_width * .5f) - (text.getLocalBounds().width * .5f), 100);
}

void GameScene::update(const float& dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        GameSystem::set_active_scene(Scenes::menu);
    }
    Scene::update(dt);
}

void GameScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

void GameScene::load() {
    em = std::make_shared<EntityManager>();

    {
        std::shared_ptr<Entity> player = std::make_shared<Entity>();
        std::shared_ptr<ShapeComponent> shape = player->add_component<ShapeComponent>();
        player->add_component<PlayerMovementComponent>();
        shape->set_shape<sf::CircleShape>(param::entity_size);
        shape->get_shape().setFillColor(sf::Color::Yellow);
        shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));
        em->list.push_back(player);
    }

    const sf::Color ghost_cols[]{ {208, 62, 25},    // red Blinky
                                 {219, 133, 28},   // orange Clyde
                                 {70, 191, 238},   // cyan Inky
                                 {234, 130, 229} }; // pink Pinky

    for (int i = 0; i < param::ghost_count; i++) {
        std::shared_ptr<Entity> ghost = std::make_shared<Entity>();
        std::shared_ptr<ShapeComponent> shape = ghost->add_component<ShapeComponent>();
        ghost->add_component<EnemyAIComponent>();
        shape->set_shape<sf::CircleShape>(param::entity_size);
        shape->get_shape().setFillColor(ghost_cols[i % param::ghost_count]);
        shape->get_shape().setOrigin(
            sf::Vector2f(param::entity_size, param::entity_size));
        em->list.push_back(ghost);
    }
}

void GameScene::respawn() {}
