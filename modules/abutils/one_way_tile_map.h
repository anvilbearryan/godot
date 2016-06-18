/* arrow_2d.h */
#ifndef ONE_WAY_TILE_MAP_H
#define ONE_WAY_TILE_MAP_H

#include "scene/2d/tile_map.h"

class OneWayTileMap : public TileMap {

	OBJ_TYPE(OneWayTileMap, TileMap)

	Vector2 one_way_collision_direction;
	float one_way_collision_max_depth;
protected:
	static void _bind_methods();
public:
	void set_one_way_collision_direction(const Vector2& p_dir);
	Vector2 get_one_way_collision_direction() const;

	void set_one_way_collision_max_depth(float p_dir);
	float get_one_way_collision_max_depth() const;

	OneWayTileMap();
};

#endif // ONE_WAY_TILE_MAP_H