#include "abmath.h"

Vector2 ABMath::clip_point_in_rect(Vector2 vec, Rect2 rect) {
	
	return Vector2(MAX(MIN(rect.pos.x + rect.size.x, vec.x), rect.pos.x), 
				MAX(MIN(rect.pos.y + rect.size.y, vec.y), rect.pos.y));
}

void ABMath::_bind_methods() {

	ObjectTypeDB::bind_method("clip_point_in_rect", &ABMath::clip_point_in_rect);
}

ABMath::ABMath() {
}