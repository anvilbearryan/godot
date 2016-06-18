#include "arrow_2d.h"

void Arrow2D::_draw_arrow() {

	draw_line(Point2(-10, 0), Point2(+9, 0), Color(1, 0.5, 0.5));
	draw_line(Point2(7, 3), Point2(+10, 0), Color(1, 0.5, 0.5));
	draw_line(Point2(7, -3), Point2(+10, 0), Color(1, 0.5, 0.5));
	draw_line(Point2(0, -10), Point2(0, +9), Color(0.5, 1, 0.5));
	draw_line(Point2(3, 7), Point2(0, +10), Color(0.5, 1, 0.5));
	draw_line(Point2(-3, 7), Point2(0, +10), Color(0.5, 1, 0.5));
}

Rect2 Arrow2D::get_item_rect() const {

	return Rect2(Point2(-10, -10), Size2(20, 20));
}

void Arrow2D::_notification(int p_what) {

	switch (p_what) {

	case NOTIFICATION_ENTER_TREE: {

		update();
	} break;
	case NOTIFICATION_DRAW: {
		if (!is_inside_tree())
			break;
		if (get_tree()->is_editor_hint())
			_draw_arrow();

	} break;
	}

}

Arrow2D::Arrow2D()
{
}