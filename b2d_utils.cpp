#include "b2d_utils.hpp"
#include "game_parameters.hpp"

using param = Parameters;


//Convert from b2Vec2 to a Vector2f
const sf::Vector2f box2d::bv2_to_sv2(const b2Vec2& in) {
	return sf::Vector2f(in.x * param::physics_scale, (in.y * param::physics_scale));
}
//Convert from Vector2f to a b2Vec2
const b2Vec2 box2d::sv2_to_bv2(const sf::Vector2f& in) {
	return { in.x * param::physics_scale_inv, in.y * param::physics_scale_inv };
}
//Convert from screenspace.y to physics.y (as they are the other way around)
const sf::Vector2f box2d::invert_height(const sf::Vector2f& in) {
	return sf::Vector2f(in.x, param::game_height - in.y);
}

//Create a Box2D body with a box fixture
b2BodyId box2d::create_physics_box(b2WorldId& world_id, const bool dynamic, const sf::Vector2f& position, const sf::Vector2f& size) {
	b2BodyDef body_def = b2DefaultBodyDef();
	//Is Dynamic(moving), or static(Stationary)
	body_def.type = dynamic ? b2_dynamicBody : b2_staticBody;
	body_def.position = sv2_to_bv2(position);
	//Create the body
	b2BodyId body_id = b2CreateBody(world_id, &body_def);

	//Create the fixture shape
	b2ShapeDef shape_def = b2DefaultShapeDef();
	shape_def.density = dynamic ? 10.f : 0.f;
	shape_def.material.friction = dynamic ? 0.8f : 1.f;
	shape_def.material.restitution = 1.0f;
	b2Polygon polygon = b2MakeBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
	b2CreatePolygonShape(body_id, &shape_def, &polygon);

	return body_id;
}

b2BodyId box2d::create_physics_box(b2WorldId& world_id, const bool dynamic, const std::shared_ptr<sf::RectangleShape>& rs) {
	return create_physics_box(world_id, dynamic, rs->getPosition(), rs->getSize());
}