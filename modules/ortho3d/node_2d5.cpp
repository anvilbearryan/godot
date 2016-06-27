#include "node_2d5.h"

void Node2D5::set_ortho_centre(const Point2& p_ortho_centre){
	ortho_centre = p_ortho_centre;
	if (get_child_count() > 0) {
		Node2D *n = get_child(0)->cast_to<Node2D>();
		if (n) {
			n->set_pos(ortho_centre);
		}
	}
	update();
}

Point2 Node2D5::get_ortho_centre() const{
	return ortho_centre;
}

void Node2D5::set_world_dim(const Vector3& p_world_dim) {
	world_dim = p_world_dim;
	update();
}

Vector3 Node2D5::get_world_dim() const {
	return world_dim;
}

Rect2 Node2D5::get_item_rect() const {
	Vector2 dim = Vector2(world_dim.x, world_dim.y + world_dim.z*0.5f); // 0.5 - skewness, TODO more organized
	return Rect2(ortho_centre - dim*0.5f, dim);
}

void Node2D5::_draw_rect() {
	draw_rect(get_item_rect(), Color(1.0, 1.0, 1.0));
}

void Node2D5::_notification(int p_what) {

	switch (p_what) {

	case NOTIFICATION_ENTER_TREE: {
		set_pos(Point2());
		update();
	} break;
	case NOTIFICATION_DRAW: {
		if (!is_inside_tree())
			break;
		_draw_rect();
	} break;
	}

}

void Node2D5::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("set_ortho_centre", "ortho_centre"), &Node2D5::set_ortho_centre);
	ObjectTypeDB::bind_method(_MD("get_ortho_centre"), &Node2D5::get_ortho_centre);

	ObjectTypeDB::bind_method(_MD("set_world_dim", "world_dim"), &Node2D5::set_world_dim);
	ObjectTypeDB::bind_method(_MD("get_world_dim"), &Node2D5::get_world_dim);

	ADD_PROPERTYNZ(PropertyInfo(Variant::VECTOR3, "2d5/world_dim"), _SCS("set_world_dim"), _SCS("get_world_dim"));
}

Node2D5::Node2D5()
{
	ortho_centre = Vector2();
	world_dim = Vector3(16, 16, 16);
}