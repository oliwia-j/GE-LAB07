#include <iostream>
#include "physics_cmps.hpp"
#include "physics.hpp"
#include "game_parameters.hpp"
#include "level_system.hpp"
#include <algorithm>

using param = Parameters;
using ph = Physics;
using ls = LevelSystem;

PlatformComponent::PlatformComponent(Entity* p, const std::vector<sf::Vector2i>& tile_group,
    float friction, float restitution)
    : Component(p), _friction(friction), _restitution(restitution) {
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_staticBody;
    //Create the body
    _body_id = b2CreateBody(ph::get_world_id(), &body_def);
    _create_chain_shape(tile_group);
}
void PlatformComponent::update(const float& dt) {}
void PlatformComponent::render() {}
PlatformComponent::~PlatformComponent() {
    b2DestroyChain(_chain_id);
    _chain_id = b2_nullChainId;
    b2DestroyBody(_body_id);
    _body_id = b2_nullBodyId;
}
void PlatformComponent::_create_chain_shape(const std::vector<sf::Vector2i>& tile_group) {
    std::vector<b2Vec2> points;
    for (int i = 0; i < tile_group.size(); i++) {
        const sf::Vector2i& tile = tile_group[i];
        std::vector<ls::Tile> neighbors = {
            ls::in_group({tile.x - 1,tile.y - 1},tile_group) ? ls::get_tile({tile.x - 1,tile.y - 1}) : ls::EMPTY,
            ls::in_group({tile.x,tile.y - 1},tile_group) ? ls::get_tile({tile.x,tile.y - 1}) : ls::EMPTY,
            ls::in_group({tile.x + 1,tile.y - 1},tile_group) ? ls::get_tile({tile.x + 1,tile.y - 1}) : ls::EMPTY,
            ls::in_group({tile.x + 1,tile.y},tile_group) ? ls::get_tile({tile.x + 1,tile.y}) : ls::EMPTY,
            ls::in_group({tile.x + 1,tile.y + 1},tile_group) ? ls::get_tile({tile.x + 1,tile.y + 1}) : ls::EMPTY,
            ls::in_group({tile.x,tile.y + 1},tile_group) ? ls::get_tile({tile.x,tile.y + 1}) : ls::EMPTY,
            ls::in_group({tile.x - 1,tile.y + 1},tile_group) ? ls::get_tile({tile.x - 1,tile.y + 1}) : ls::EMPTY,
            ls::in_group({tile.x - 1,tile.y},tile_group) ? ls::get_tile({tile.x - 1,tile.y}) : ls::EMPTY
        };
        sf::Vector2f pos = ls::get_tile_position(tile);

        std::vector<sf::Vector2f> pts;
        if (neighbors[0] == ls::EMPTY || neighbors[1] == ls::EMPTY || neighbors[7] == ls::EMPTY)
            pts.push_back(pos);
        if (neighbors[1] == ls::EMPTY || neighbors[2] == ls::EMPTY || neighbors[3] == ls::EMPTY)
            pts.push_back({ pos.x + param::tile_size,pos.y });
        if (neighbors[3] == ls::EMPTY || neighbors[4] == ls::EMPTY || neighbors[5] == ls::EMPTY)
            pts.push_back({ pos.x + param::tile_size,pos.y + param::tile_size });
        if (neighbors[5] == ls::EMPTY || neighbors[6] == ls::EMPTY || neighbors[7] == ls::EMPTY)
            pts.push_back({ pos.x,pos.y + param::tile_size });

        for (const sf::Vector2f& pt : pts) {
            b2Vec2 point = ph::sv2_to_bv2(ph::invert_height(pt, param::game_height));
            bool already_in = false;
            for (const b2Vec2& p : points) {
                if (p.x == point.x && p.y == point.y) {
                    already_in = true;
                    break;
                }
            }
            if (!already_in)
                points.push_back(point);
        }
    }
    b2Vec2 centroid = { 0,0 };
    for (const b2Vec2 pt : points) {
        centroid.x += pt.x;
        centroid.y += pt.y;
    }
    centroid.x /= static_cast<float>(points.size());
    centroid.y /= static_cast<float>(points.size());
    //order the list of points in counter clockwise.
    std::sort(points.begin(), points.end(), [&](b2Vec2 a, b2Vec2 b) {
        a = { a.x - centroid.x,a.y - centroid.y };
        b = { b.x - centroid.x,b.y - centroid.y };
        float angle1 = std::atan2(a.x, a.y);
        float angle2 = std::atan2(b.x, b.y);
        if (angle1 == angle2)
            return std::sqrt(a.x * a.x + a.y * a.y) > std::sqrt(b.x * b.x + b.y * b.y);
        else
            return angle1 > angle2;
        });
    points.push_back(points.front());

    b2SurfaceMaterial material = b2DefaultSurfaceMaterial();
    material.friction = _friction;
    material.restitution = _restitution;
    b2ChainDef chain_def = b2DefaultChainDef();
    chain_def.count = points.size();
    chain_def.points = points.data();
    chain_def.isLoop = true;
    chain_def.materials = &material;
    chain_def.materialCount = 1;
    _chain_id = b2CreateChain(_body_id, &chain_def);
    std::vector<b2ShapeId> shape_ids(points.size());
    int nbr_seg = b2Chain_GetSegments(_chain_id, shape_ids.data(), points.size());
    shape_ids.size();
}

void PhysicsComponent::update(const float& dt) {
    //std::cout << dt << std::endl;
    _parent->set_position(ph::invert_height(ph::bv2_to_sv2(b2Body_GetPosition(_body_id)),
        param::game_height));
    _parent->set_rotation((180 / M_PI) * b2Rot_GetAngle(b2Body_GetRotation(_body_id)));
}

PhysicsComponent::PhysicsComponent(Entity* p, bool dyn)
    : Component(p), _dynamic(dyn) {

    b2BodyDef body_def = b2DefaultBodyDef();
    //Is Dynamic(moving), or static(Stationary)
    body_def.type = _dynamic ? b2_dynamicBody : b2_staticBody;
    body_def.position = ph::sv2_to_bv2(ph::invert_height(_parent->get_position(), param::game_height));

    //Create the body
    _body_id = b2CreateBody(ph::get_world_id(), &body_def);
}

void PhysicsComponent::set_restitution(float r) {
    _restitution = r;
    b2Shape_SetRestitution(_shape_id, r);
}

void PhysicsComponent::set_friction(float f) {
    _friction = f;
    b2Shape_SetFriction(_shape_id, f);
}

void PhysicsComponent::set_mass(float m) {
    _mass = m;
    b2Shape_SetDensity(_shape_id, m, true);
}

void PhysicsComponent::teleport(const sf::Vector2f& v) {
    b2CosSin cos_sin = b2ComputeCosSin(0.0f);
    b2Rot rot;
    rot.c = cos_sin.cosine;
    rot.s = cos_sin.sine;
    b2Body_SetTransform(_body_id, ph::sv2_to_bv2(ph::invert_height(v, param::game_height)), rot);
}

const sf::Vector2f PhysicsComponent::get_velocity() const {
    return ph::bv2_to_sv2(b2Body_GetLinearVelocity(_body_id));
}
void PhysicsComponent::set_velocity(const sf::Vector2f& v) {
    b2Body_SetLinearVelocity(_body_id, ph::sv2_to_bv2(v));
}

const b2ShapeId& PhysicsComponent::get_shape_id() const { return _shape_id; }

PhysicsComponent::~PhysicsComponent() {
    b2DestroyShape(_shape_id, true);
    _shape_id = b2_nullShapeId;
    b2DestroyBody(_body_id);
    _body_id = b2_nullBodyId;
}

void PhysicsComponent::render() {}

void PhysicsComponent::impulse(const sf::Vector2f& i) {
    b2Vec2 a;
    a.x = i.x;
    a.y = i.y * -1.f;
    b2Body_ApplyLinearImpulseToCenter(_body_id, a, true);
    auto vel = b2Body_GetLinearVelocity(_body_id);
}

void PhysicsComponent::dampen(const sf::Vector2f& i) {
    auto vel = b2Body_GetLinearVelocity(_body_id);
    vel.x *= i.x;
    vel.y *= i.y;
    b2Body_SetLinearVelocity(_body_id, vel);
}

int PhysicsComponent::get_contacts(std::array<b2ContactData, 10>& contacts) const {
    int contact_count = b2Body_GetContactData(_body_id, contacts.data(), 10);
    return contact_count;
}

void PhysicsComponent::create_box_shape(const sf::Vector2f& size, float mass, float friction, float restitution) {
    _mass = mass;
    _friction = friction;
    _restitution = restitution;
    //Create the fixture shape
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = _dynamic ? _mass : 0.f;
    shape_def.material.friction = _friction;
    shape_def.material.restitution = _restitution;
    b2Polygon polygon = b2MakeBox(ph::sv2_to_bv2(size).x * 0.5f, ph::sv2_to_bv2(size).y * 0.5f);
    _shape_id = b2CreatePolygonShape(_body_id, &shape_def, &polygon);
}

void PhysicsComponent::create_capsule_shape(const sf::Vector2f& size, float mass, float friction, float restitution) {
    _mass = mass;
    _friction = friction;
    _restitution = restitution;
    //Create the fixture shape
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = _dynamic ? _mass : 0.f;
    shape_def.material.friction = _friction;
    shape_def.material.restitution = _restitution;
    b2Vec2 b2_size = ph::sv2_to_bv2(size);
    b2Capsule capsule;
    capsule.center1 = { 0,b2_size.y * 0.5f - b2_size.x * 0.5f };
    capsule.center2 = { 0,-b2_size.y * 0.5f + b2_size.x * 0.5f };
    capsule.radius = b2_size.x * 0.5f;
    _shape_id = b2CreateCapsuleShape(_body_id, &shape_def, &capsule);
}


PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
    const sf::Vector2f& size)
    : PhysicsComponent(p, true) {
    _size = ph::sv2_to_bv2(size);
    _max_velocity = sf::Vector2f(param::player_max_vel[0], param::player_max_vel[1]);
    _ground_speed = param::player_impulse;
    _grounded = false;
    b2Body_EnableSleep(_body_id, false);
    b2Body_SetFixedRotation(_body_id, true);
    //Bullet items have higher-res collision detection
    // b2Body_SetBullet(_body_id,true);
}

bool PlayerPhysicsComponent::is_grounded() const {
    std::array<b2ContactData, 10> contacts;
    int count = get_contacts(contacts);
    if (count <= 0)
        return false;
    const b2Vec2& pos = b2Body_GetPosition(_body_id);
    const float half_y = _size.y * .5f;
    for (int i = 0; i < count; i++) {
        if (contacts[i].manifold.normal.y == 1)
            return true;
    }

    return false;
}

void PlayerPhysicsComponent::update(const float& dt) {
    const sf::Vector2f pos = _parent->get_position();
    b2Vec2 b2_pos = ph::sv2_to_bv2(ph::invert_height(pos, param::game_height));

    //Teleport to start if we fall off map.
    if (pos.y > ls::get_height() * param::tile_size) {
        teleport(ls::get_start_position());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // Moving Either Left or Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            impulse({ (dt * _ground_speed), 0 });
        }
        else {
            impulse({ -(dt * _ground_speed), 0 });
        }
    }
    else {
        // Dampen X axis movement
        dampen({ 0.9f, 1.0f });
    }

    // Handle Jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        _grounded = is_grounded();
        if (_grounded) {
            set_velocity(sf::Vector2f(get_velocity().x, 0.f));
            teleport(sf::Vector2f(pos.x, pos.y - 2.0f));
            impulse(sf::Vector2f(0, -param::player_jump));
        }
    }
    //Are we in air?
    if (!_grounded) {
        // Check to see if we have landed yet
        _grounded = is_grounded();
        // disable friction while jumping
        set_friction(0.f);
    }
    else {
        set_friction(_friction);
    }

    // Clamp velocity.
    sf::Vector2f v = get_velocity();
    v.x = copysign(std::min(abs(v.x), _max_velocity.x), v.x);
    v.y = copysign(std::min(abs(v.y), _max_velocity.y), v.y);
    set_velocity(v);

    PhysicsComponent::update(dt);
}