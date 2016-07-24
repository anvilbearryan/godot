/* anchored_sprite.h */
#ifndef ANCHORED_SPRITE_H
#define ANCHORED_SPRITE_H

#include "scene/2d/sprite.h"

class AnchoredSprite : public Sprite {

	OBJ_TYPE(AnchoredSprite, Sprite);

	int anchor;

protected:
	static void _bind_methods();

public:
	void set_anchor(int p_anchor);
	int get_anchor() const;
	virtual void adjust_offset(Point2& in_offset, const Size2& ref_size) const override;
	AnchoredSprite();
};

#endif // ANCHORED_SPRITE_H