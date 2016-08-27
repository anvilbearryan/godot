/*************************************************************************/
/*  anchored_container.cpp                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "anchored_container.h"


Size2 AnchoredContainer::get_minimum_size() const {


	if (!bounded)
		return Size2();
	Size2 ms;
	for(int i=0;i<get_child_count();i++) {

		Control *c = get_child(i)->cast_to<Control>();
		if (!c)
			continue;
		if (c->is_set_as_toplevel())
			continue;
		if (c->is_hidden())
			continue;
		Size2 minsize = c->get_combined_minimum_size();
		ms.width = MAX(ms.width , minsize.width);
		ms.height = MAX(ms.height , minsize.height);


	}

	return ms;

}


void AnchoredContainer::set_anchored_to(int p_anchor) {

	anchor=p_anchor;
	queue_sort();

}

int AnchoredContainer::get_anchored_to() const {

	return anchor;
}

void AnchoredContainer::set_bounded(bool p_bounded) {
	bounded = p_bounded;
	queue_sort();
}

bool AnchoredContainer::is_bounded() const {
	return bounded;
}

void AnchoredContainer::_notification(int p_what) {

	if (p_what==NOTIFICATION_SORT_CHILDREN) {

		Size2 size = get_size();
		for(int i=0;i<get_child_count();i++) {

			Control *c = get_child(i)->cast_to<Control>();
			if (!c)
				continue;
			if (c->is_set_as_toplevel())
				continue;

			Size2 minsize = c->get_combined_minimum_size();
			// Begin custom
			Point2 ofs = Point2();
			//ofs.y = (2 - (anchor - 1) / 3) * size.y / 2;
			ofs.y = (2 - (anchor - 1) / 3) * size.y / 2;
			ofs.x = ((anchor - 1) % 3) * size.x / 2;
			// bound adjust, want a function s.t. 5 gives 0.5,0.5
			if (!bounded) {
				ofs = ofs - (minsize*0.5).floor();
			}
			else {
				ofs.y -= (2 - (anchor - 1) / 3) * minsize.y / 2;
				ofs.x -= ((anchor - 1) % 3) * minsize.x / 2;
			}
			// End custom
			fit_child_in_rect(c, Rect2(ofs, minsize));
		}
	}
}

void AnchoredContainer::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_anchored_to","anchored_to"),&AnchoredContainer::set_anchored_to);
	ObjectTypeDB::bind_method(_MD("get_anchored_to"),&AnchoredContainer::get_anchored_to);

	ObjectTypeDB::bind_method(_MD("set_bounded", "enable_bound"), &AnchoredContainer::set_bounded);
	ObjectTypeDB::bind_method(_MD("is_bounded"), &AnchoredContainer::is_bounded);

	ADD_PROPERTYNZ(PropertyInfo(Variant::INT, "anchored_to"), _SCS("set_anchored_to"), _SCS("get_anchored_to"));
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bounded"), _SCS("set_bounded"), _SCS("is_bounded"));
}

AnchoredContainer::AnchoredContainer() {
	anchor=7;
	bounded = true;
}
