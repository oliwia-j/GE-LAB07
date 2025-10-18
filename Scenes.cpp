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
    font.loadFromFile(param::font_path);
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
    ls::render(Renderer::get_window());
    Scene::render();
}

void GameScene::load() {
    ls::load_level(param::pacman_map, param::tile_size);

    em = std::make_shared<EntityManager>();

    {
        std::shared_ptr<Entity> player = std::make_shared<Entity>();
        std::shared_ptr<ShapeComponent> shape = player->add_component<ShapeComponent>();
        player->add_component<PlayerMovementComponent>();
        shape->set_shape<sf::CircleShape>(param::entity_size);
        shape->get_shape().setFillColor(sf::Color::Yellow);
        shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));
        
        em->set_player(player);
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
        em->add_enemy(ghost);
    }

    respawn();
}

void GameScene::respawn() {
    std::shared_ptr<Entity> player = em->get_player();
    std::vector<std::shared_ptr<Entity>> enemies = em->get_enemies();

    player->set_position(ls::get_start_position());
    player->get_compatible_components<ActorMovementComponent>()[0]
        ->set_speed(param::player_speed);

    std::vector<sf::Vector2i> ghost_spawns = ls::find_tiles(ls::ENEMY);
    for (size_t i = 1; i < enemies.size(); i++) {
        std::shared_ptr<Entity>& ghost = enemies[i];
        ghost->set_position(
            ls::get_tile_position(ghost_spawns[rand() % ghost_spawns.size()]));
        ghost->get_compatible_components<ActorMovementComponent>()[0]->set_speed(param::ghost_speed);
    }
    // ...
}