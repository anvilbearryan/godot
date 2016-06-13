/* arrow_2d.h */
#ifndef ARROW_2D_H
#define ARROW_2D_H

#include "scene/2d/node_2d.h"

class Arrow2D : public Node2D {

	OBJ_TYPE(Arrow2D, Node2D)

		void _draw_arrow();
protected:

	void _notification(int p_what);
public:

	virtual Rect2 get_item_rect() const;
	Arrow2D();
};

#endif // ARROW_2D_H