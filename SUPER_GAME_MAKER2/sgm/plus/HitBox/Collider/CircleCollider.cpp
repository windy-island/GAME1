#include "CircleCollider.h"
#include "RectCollider.h"
#include "PointCollider.h"
#include "TileMapCollider.hpp"
#include "CollisionAlgorithms.h"

namespace SGM2::plus::hitbox::collider {

	// 矩形との衝突判定
	const CollisionResult CircleCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const RectCollider& targetCollider) const {
		return { overlap_circle_vs_rect(
			objectCenterPos + offset,
			r,
			targetCenterPos + targetCollider.get_offset() - targetCollider.get_size() / 2,
			targetCollider.get_size()
		), {} };
	}

	// 円との衝突判定
	const CollisionResult CircleCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const CircleCollider& targetCollider) const {
		return { overlap_circle_vs_circle(
			objectCenterPos + offset,
			r,
			targetCenterPos + targetCollider.offset,
			targetCollider.r
		), {} };
	}

	// 点との衝突判定
	const CollisionResult CircleCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const PointCollider& targetCollider) const {
		return { overlap_point_vs_circle(
			targetCenterPos + targetCollider.get_offset(),
			objectCenterPos + offset,
			r
		), {} };
	}

	// タイルマップとの衝突判定を行う ★★★★★★★★★
	const CollisionResult CircleCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const TileMapCollider& targetCollider) const {
		return { false, {} };
	}
}
