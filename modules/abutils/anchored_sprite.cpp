#include "anchored_sprite.h"

void AnchoredSprite::set_anchor(int p_anchor) {
	ERR_FAIL_COND(p_anchor < 1 || p_anchor>9);
	anchor = p_anchor;
	update();
	item_rect_changed();
}

int AnchoredSprite::get_anchor() const {
	return anchor;
}

void AnchoredSprite::adjust_offset(Point2& in_offset, const Size2& ref_size) const {
	in_offset.y -= (2 - (anchor - 1) / 3) * ref_size.y / 2;
	in_offset.x -= ((anchor - 1) % 3) * ref_size.x / 2;
}

void AnchoredSprite::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_anchor", "anchor"), &AnchoredSprite::set_anchor);
	ObjectTypeDB::bind_method(_MD("get_anchor"), &AnchoredSprite::get_anchor);

	ADD_PROPERTYNZ(PropertyInfo(Variant::INT, "anchor"), _SCS("set_anchor"), _SCS("get_anchor"));
}

AnchoredSprite::AnchoredSprite() {
	anchor = 7;
}