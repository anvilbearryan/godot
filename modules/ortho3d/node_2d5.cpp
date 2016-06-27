#include "node_2d5.h"

void Node2D5::set_ortho_pos(const Point2& p_ortho_pos){
	ortho_pos = p_ortho_pos;
	if (get_child_count() > 0) {
		Node2D *n = get_child(0)->cast_to<Node2D>();
		if (n) {
			n->set_pos(ortho_pos);
		}
	}
	update();
}

Point2 Node2D5::get_ortho_pos() const{
	return ortho_pos;
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
	return Rect2(ortho_pos - dim*0.5f, dim);
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
	ObjectTypeDB::bind_method(_MD("set_ortho_pos", "ortho_pos"), &Node2D5::set_ortho_pos);
	ObjectTypeDB::bind_method(_MD("get_ortho_pos"), &Node2D5::get_ortho_pos);

	ObjectTypeDB::bind_method(_MD("set_world_dim", "world_dim"), &Node2D5::set_world_dim);
	ObjectTypeDB::bind_method(_MD("get_world_dim"), &Node2D5::get_world_dim);

	ADD_PROPERTYNZ(PropertyInfo(Variant::VECTOR2, "2d5/ortho_pos"), _SCS("set_ortho_pos"), _SCS("get_ortho_pos"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::VECTOR3, "2d5/world_dim"), _SCS("set_world_dim"), _SCS("get_world_dim"));
}

Node2D5::Node2D5()
{
	ortho_pos = Vector2();
	world_dim = Vector3(16, 16, 16);
}