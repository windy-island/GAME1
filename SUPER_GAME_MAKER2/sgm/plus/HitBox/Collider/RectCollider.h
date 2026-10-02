#pragma once

#include "Collider.hpp"

namespace SGM2::plus::hitbox::collider {

	class RectCollider : public Collider {

		Vec2I offset;	// オブジェクト位置からのオフセット
		Size size;		// 矩形のサイズ

	public:
		RectCollider(const Vec2I& offset_, const Size& size_) :
			Collider(ColliderType::Rect),
			offset(offset_),
			size(size_) {
		}

	public:
		const Vec2I get_offset() const { return offset; }

		const Size get_size() const { return size; }

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
			Rect{ Arg::center((dp.pos + offset * dp.scale).asPoint()), (size * dp.scale).asPoint() }.draw(color);
		}
	};
}
