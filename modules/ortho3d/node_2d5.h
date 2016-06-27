/* node_2d5.h */
#ifndef NODE_2D5_H
#define NODE_2D5_H

#include "scene/2d/node_2d.h"

// extends Node2D so as to allow drawing debug rects
class Node2D5 : public Node2D {

	OBJ_TYPE(Node2D5, Node2D)

	Point2 ortho_pos;
	Vector3 world_dim;

	void _draw_rect();

protected:
	void _notification(int p_what);
	static void _bind_methods();
public:
	void set_ortho_pos(const Point2& p_ortho_pos);
	Point2 get_ortho_pos() const;
	
	void set_world_dim(const Vector3& p_world_dim);
	Vector3 get_world_dim() const;

	virtual Rect2 get_item_rect() const;

	Node2D5();
};

#endif // NODE_2D5_H