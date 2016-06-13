/* abmath.h */
#ifndef ABMATH_H
#define ABMATH_H

#include "scene/main/node.h"

class ABMath : public Node {

	OBJ_TYPE(ABMath, Node)

protected:
	static void _bind_methods();

public:
	Vector2 clip_point_in_rect(Vector2 vec, Rect2 rect);
	ABMath();
};

#endif // ABMATH_H