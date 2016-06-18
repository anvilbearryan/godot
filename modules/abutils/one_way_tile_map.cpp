#include "one_way_tile_map.h"

void OneWayTileMap::set_one_way_collision_direction(const Vector2& p_dir) {

	one_way_collision_direction = p_dir;
	
	for (Map<PosKey, Quadrant>::Element *E = quadrant_map.front();E;E = E->next()) {

		Quadrant &q = E->get();
		Physics2DServer::get_singleton()->body_set_one_way_collision_direction(q.body, p_dir);
	}
}

Vector2 OneWayTileMap::get_one_way_collision_direction() const {

	return one_way_collision_direction;
}


void OneWayTileMap::set_one_way_collision_max_depth(float p_depth) {

	one_way_collision_max_depth = p_depth;

	for (Map<PosKey, Quadrant>::Element *E = quadrant_map.front();E;E = E->next()) {

		Quadrant &q = E->get();
		Physics2DServer::get_singleton()->body_set_one_way_collision_max_depth(q.body, p_depth);
	}

}

float OneWayTileMap::get_one_way_collision_max_depth() const {

	return one_way_collision_max_depth;
}

void OneWayTileMap::_bind_methods(){
	ObjectTypeDB::bind_method(_MD("set_one_way_collision_direction", "dir"), &OneWayTileMap::set_one_way_collision_direction);
	ObjectTypeDB::bind_method(_MD("get_one_way_collision_direction"), &OneWayTileMap::get_one_way_collision_direction);
	ObjectTypeDB::bind_method(_MD("set_one_way_collision_max_depth", "depth"), &OneWayTileMap::set_one_way_collision_max_depth);
	ObjectTypeDB::bind_method(_MD("get_one_way_collision_max_depth"), &OneWayTileMap::get_one_way_collision_max_depth);

	ADD_PROPERTYNZ(PropertyInfo(Variant::VECTOR2, "one_way_collision/direction"), _SCS("set_one_way_collision_direction"), _SCS("get_one_way_collision_direction"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::REAL, "one_way_collision/max_depth"), _SCS("set_one_way_collision_max_depth"), _SCS("get_one_way_collision_max_depth"));
}

OneWayTileMap::OneWayTileMap() :TileMap(){

	set_one_way_collision_max_depth(0);

}
