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

// maps to ranges (-PI, PI]
float standardize_range(float in) {
	const float PI = 3.1415927f;
	const float PI2 = PI * 2.0f;
	while (in > PI) {
		in -= PI2;
	}
	while (in <= -PI) {
		in += PI2;
	}
	return in;
}

float ABMath::lerp_rot_rad(float arg_from, float arg_to, float arg_abs_amount){
	// for clarity only, it should suffice to just do standardize_range(arg_to - arg_from);
	float from = standardize_range(arg_from);
	float to = standardize_range(arg_to);
	float delta = standardize_range(to - from);
	if (delta > arg_abs_amount) {
		return standardize_range(from + arg_abs_amount);
	}
	else if (delta < -arg_abs_amount) {
		return standardize_range(from - arg_abs_amount);
	}
	else
	{
		return standardize_range(from + delta);
	}
}
float ABMath::lerp_rot_deg(float arg_from, float arg_to, float arg_abs_amount) {
	return lerp_rot_rad(Math::deg2rad(arg_from), Math::deg2rad(arg_to), Math::deg2rad(arg_abs_amount));
}

void ABMath::_bind_methods() {

	ObjectTypeDB::bind_method("clip_point_in_rect", &ABMath::clip_point_in_rect);
	ObjectTypeDB::bind_method("fast_atan2", &ABMath::fast_atan2);
	ObjectTypeDB::bind_method("fast_atanv", &ABMath::fast_atanv);
	ObjectTypeDB::bind_method("lerp_rot_rad", &ABMath::lerp_rot_rad);
	ObjectTypeDB::bind_method("lerp_rot_deg", &ABMath::lerp_rot_deg);
}

ABMath::ABMath() {
}