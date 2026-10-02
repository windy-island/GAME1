#pragma once

#include "Collider.hpp"

namespace SGM2::plus::hitbox::collider {

	class CircleCollider: public Collider {

		Vec2I offset;	// オブジェクト位置からのオフセット
		int32 r;		// 円の半径

	public:
		CircleCollider(const Vec2I& offset_, const int32 r_) :
			Collider(ColliderType::Circle),
			offset(offset_),
			r(r_) {
		}

	public:
		const Vec2I get_offset() const { return offset; }

		const int32 get_radius() const { return r; }

	private:
		// 矩形との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const RectCollider& targetCollider) const override;

		// 円との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const CircleCollider& targetCollider) const override;

		// 点との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const PointCollider& targetCollider) const override;

		// タイルマップとの衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const TileMapCollider& targetCollider) const override;

		// デバッグ描画
		void debug_draw(const DrawingProperties& dp, const Color& color) const override {
			Circle{ (dp.pos + offset * dp.scale).asPoint(), (int32)(r * dp.scale) }.draw(color);
		}
	};
}
