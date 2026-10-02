#pragma once

#include "sgmPlus.h"

class ObjC : public Object {

public:
	ObjC(const Vec3& pos_, const Vec2I& offset_, CollisionManager& cm_) : Object(pos_), cm(cm_), offset(offset_) {}

private:
	CollisionManager& cm;
	shared_ptr<HitBoxComponent> hitbox;
	const Vec2I offset;

	void ready() override {
		hitbox = add_component(cm.create_hitbox(*this));
		auto& cset = hitbox->add_collider_set("body");
		cset.add_point_collider(offset);
		hitbox->activate("body");
		cset.set_visible_flag(true);
		hitbox->set_debug_color(ColorF(1, 1, 0, 0.6));
	}

	bool update() override {
		if (hitbox->is_hit()) {
			hitbox->set_debug_color(ColorF(1, 0.5, 0, 0.6));
		}
		else {
			hitbox->set_debug_color(ColorF(1, 1, 0, 0.6));
		}

		return true;
	}

	void draw(const DrawingProperties& dp) const override {
		Circle(dp.pos, 2 * dp.scale).draw(ColorF(0, 1, 1));
		for (const auto& id : hitbox->get_sub_collision_dataA()) {
			Print << U"TileId: " << id;
		}
	}
};

