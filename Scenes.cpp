#include "Scenes.hpp"
#include "Player.hpp"
#include "Ghost.hpp"
#include "Renderer.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "level_system.hpp"

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
}

void GameScene::respawn() {}
