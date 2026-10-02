#include "PointCollider.h"
#include "RectCollider.h"
#include "CircleCollider.h"
#include "TileMapCollider.hpp"
#include "CollisionAlgorithms.h"

namespace SGM2::plus::hitbox::collider {

	// 矩形との衝突判定
	const CollisionResult PointCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const RectCollider& targetCollider) const {
		return {
			overlap_point_vs_rect(
				objectCenterPos + offset,
				targetCenterPos + targetCollider.get_offset() - targetCollider.get_size() / 2,
				targetCollider.get_size()
			),
			{}
		};
	}

	// 円との衝突判定
	const CollisionResult PointCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const CircleCollider& targetCollider) const {
		return {
			overlap_point_vs_circle(
				objectCenterPos + offset,
				targetCenterPos + targetCollider.get_offset(),
				targetCollider.get_radius()
			),
			{}
		};
	}

	// 点との衝突判定
	const CollisionResult PointCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const PointCollider& targetCollider) const {
		return { overlap_point_vs_point(
			objectCenterPos + offset,
			targetCenterPos + targetCollider.get_offset()
		), {} };
	}

	// タイルマップとの衝突判定を行う
	const CollisionResult PointCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const TileMapCollider& targetCollider) const {
		return overlap_point_vs_tile_map(
			(objectCenterPos + offset) - targetCenterPos,
			targetCollider.get_layer_array(),
			targetCollider.get_tile_size()
		);
	}
}
