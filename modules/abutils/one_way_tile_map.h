/* arrow_2d.h */
#ifndef ONE_WAY_TILE_MAP_H
#define ONE_WAY_TILE_MAP_H

#include "scene/2d/tile_map.h"

class OneWayTileMap : public TileMap {

	OBJ_TYPE(OneWayTileMap, TileMap)

		void _draw_arrow();
protected:

	void _notification(int p_what);
public:

	virtual Rect2 get_item_rect() const;
	Arrow2D();
};

#endif // ARROW_2D_H