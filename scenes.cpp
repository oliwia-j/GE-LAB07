#include <cmath>
#include "scenes.hpp"
#include "game_parameters.hpp"
#include "components.hpp"
#include "renderer.hpp"
#include "level_system.hpp"

using ls = LevelSystem;
using gs = GameSystem;
using param = Parameters;

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::game;

void MenuScene::update(const float &dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      gs::set_active_scene(Scenes::game);
  }
  Scene::update(dt);
  _text.setString("Almost Pacman");
}

void MenuScene::render() {
  Renderer::queue(&_text);
  Scene::render();
}

void MenuScene::load() {
  _font.loadFromFile(param::font_path);
  _text.setFont(_font);
  _text.setCharacterSize(60);
}


void GameScene::update(const float &dt){
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
      gs::set_active_scene(Scenes::menu);
  }
    auto vect_distance = [](sf::Vector2f a,sf::Vector2f b) -> float{
      return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    };
  for (const std::shared_ptr<Entity> &ghost: _ghosts) {
    if (vect_distance(ghost->get_position(), _player->get_position()) < 30.0f) {
        respawn();
    }
  }
    Scene::update(dt);
    _nibbles.update(dt);
}

void GameScene::render(){
  ls::render(Renderer::get_window());
  Scene::render();
  _entities.render();
  _nibbles.render();

}

void GameScene::load() {

  ls::load_level(param::pacman_map_path,param::tile_size);

  {
    std::shared_ptr<Entity> player = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = player->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(param::entity_size);
    shape->get_shape().setFillColor(sf::Color::Yellow);
    shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));  

    player->add_component<PlayerMovementComponent>();
    player->set_position(ls::get_start_position());
    _entities.list.push_back(player);
    _player = _entities.list.back();
  }

  const sf::Color ghost_cols[]{{208, 62, 25},    // red Blinky
                               {219, 133, 28},   // orange Clyde
                               {70, 191, 238},   // cyan Inky
                               {234, 130, 229}}; // pink Pinky

  for(int i = 0; i < param::number_ghosts; i++){
    std::shared_ptr<Entity> ghost = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = ghost->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(param::entity_size);
    shape->get_shape().setFillColor(ghost_cols[i % param::number_ghosts]);
    shape->get_shape().setOrigin(sf::Vector2f(param::entity_size, param::entity_size));
    ghost->add_component<EnemyAIComponent>();
    _ghosts.push_back(ghost);
    _entities.list.push_back(ghost);
  }
  respawn();
}

void GameScene::respawn() {
 _player->set_position(ls::get_start_position());
 _player->get_compatible_components<ActorMovementComponent>()[0]
            ->set_speed(param::player_speed);

 std::vector<sf::Vector2i> ghost_spawns = ls::find_tiles(ls::ENEMY);
 for (size_t i = 1; i < _entities.list.size(); i++) {
    std::shared_ptr<Entity> &ghost = _entities.list[i];
    ghost->set_position(
        ls::get_tile_position(ghost_spawns[rand() % ghost_spawns.size()]));
    ghost->get_compatible_components<ActorMovementComponent>()[0]->set_speed(param::ghost_speed);
 }
 
  //clear any remaining nibbles
  for (auto n : _nibbles.list) {
    n->set_for_delete();
    n.reset();
  }
  _nibbles.list.clear();

    //white nibbles
  std::vector<sf::Vector2i> nibbleLoc = ls::find_tiles(ls::EMPTY);
  for (const sf::Vector2i& nl : nibbleLoc) {
    std::shared_ptr<Entity> cherry = _make_nibble(nl, false);
    //add to _wnts and nibbles list
    _nibbles.list.push_back(cherry);
  }
  //blue nibbles
  nibbleLoc = ls::find_tiles(ls::WAYPOINT);
  for (const sf::Vector2i& nl : nibbleLoc) {
    std::shared_ptr<Entity> cherry = _make_nibble(nl, true);
    _nibbles.list.push_back(cherry);
  }
}

std::shared_ptr<Entity> GameScene::_make_nibble(const sf::Vector2i& pos, bool big) {
  std::shared_ptr<Entity> cherry = std::make_shared<Entity>();
  std::shared_ptr<ShapeComponent> s = cherry->add_component<ShapeComponent>();
  //set colour
  if(big){
    s->set_shape<sf::CircleShape>(param::big_nibble_size);
    s->get_shape().setFillColor(sf::Color::Blue);
    s->get_shape().setOrigin(param::big_nibble_size,param::big_nibble_size);
  }else{
    s->set_shape<sf::CircleShape>(param::small_nibble_size);
    s->get_shape().setFillColor(sf::Color::White);
    s->get_shape().setOrigin(param::small_nibble_size,param::small_nibble_size);
  }
  
  cherry->add_component<PickupComponent>(big);
  cherry->set_position(ls::get_tile_position(pos) + sf::Vector2f(10.f, 10.f));
  return cherry;
}