#include "abmath.h"
#include <math.h>

Vector2 ABMath::clip_point_in_rect(Vector2 vec, Rect2 rect) {
	
	return Vector2(MAX(MIN(rect.pos.x + rect.size.x, vec.x), rect.pos.x), 
				MAX(MIN(rect.pos.y + rect.size.y, vec.y), rect.pos.y));
}

float ABMath::fast_atan2(float y, float x) {
	const float PI = 3.1415927f;
	const float PI2 = PI * 2.0f;

	if (x == 0.0f) {
		if (y > 0.0f) return PI / 2.0f;
		if (y == 0.0f) return 0.0f;
		return PI / (-2.0f);
	}
	float z = y / x;
	if (Math::abs(z) < 1.0f) {
		float atan = z / (1.0f + 0.28f * z * z);
		if (x < 0.0f) {
			return atan + (y < 0.0f ? -PI : PI);
		}
		return atan;
	}
	float atan = PI / 2.0 - z / (z * z + 0.28f);
	return y < 0.0f ? atan - PI : atan;
}

float ABMath::fast_atanv(Vector2 point) {
	return fast_atan2(point.y, point.x);
}

void ABMath::_bind_methods() {

	ObjectTypeDB::bind_method("clip_point_in_rect", &ABMath::clip_point_in_rect);
	ObjectTypeDB::bind_method("fast_atan2", &ABMath::fast_atan2);
	ObjectTypeDB::bind_method("fast_atanv", &ABMath::fast_atanv);
}

ABMath::ABMath() {
}