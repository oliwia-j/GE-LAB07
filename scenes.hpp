#pragma once
#include <memory>
#include "game_system.hpp"



class MenuScene : public Scene {
private:
  sf::Text _text;
  sf::Font _font;

public:
  MenuScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load()override;
};

class GameScene : public Scene {
private:
  sf::Text _text;
  sf::Clock _score_clock;
  std::shared_ptr<Entity> _player;
  std::vector<std::shared_ptr<Entity>> _ghosts;
  EntityManager _nibbles;
  std::shared_ptr<Entity> _make_nibble(const sf::Vector2i& pos, bool big);
  void respawn();

public:
  GameScene() = default;
  void update(const float &dt) override;
  void render() override;
  void load() override;
};

struct Scenes{
  static std::shared_ptr<Scene> menu;
  static std::shared_ptr<Scene> game;
};