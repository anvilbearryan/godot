/* anchored_animated_sprite.h */
#ifndef ANCHORED_ANIMATED_SPRITE_H
#define ANCHORED_ANIMATED_SPRITE_H

#include "scene/2d/animated_sprite.h"

class AnchoredAnimatedSprite : public AnimatedSprite {

	OBJ_TYPE(AnchoredAnimatedSprite, AnimatedSprite);
		
	int anchor;
protected:
	static void _bind_methods();

public:
	void set_anchor(int p_anchor);
	int get_anchor() const;

	virtual void adjust_offset(Point2& in_offset, const Size2i& ref_size) const override;
	AnchoredAnimatedSprite();
};

#endif // ANCHORED_ANIMATED_SPRITE_H