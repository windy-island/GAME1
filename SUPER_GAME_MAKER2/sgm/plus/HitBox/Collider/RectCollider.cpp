#include "RectCollider.h"
#include "CircleCollider.h"
#include "PointCollider.h"
#include "TileMapCollider.hpp"
#include "CollisionAlgorithms.h"

namespace SGM2::plus::hitbox::collider {

	// 矩形との衝突判定
	const CollisionResult RectCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const RectCollider& targetCollider) const {
		return { overlap_rect_vs_rect(
			objectCenterPos + offset - (size / 2),
			size,
			targetCenterPos + targetCollider.get_offset() - (targetCollider.get_size() / 2),
			targetCollider.get_size()
		), {} };
	}

	// 円との衝突判定
	const CollisionResult RectCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const CircleCollider& targetCollider) const {
		return { overlap_circle_vs_rect(
			targetCenterPos + targetCollider.get_offset(),
			targetCollider.get_radius(),
			objectCenterPos + offset - (size / 2),
			size
		), {} };
	}

	// 点との衝突判定
	const CollisionResult RectCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const PointCollider& targetCollider) const {
		return { overlap_point_vs_rect(
			targetCenterPos + targetCollider.get_offset(),
			objectCenterPos + offset - (size / 2),
			size
		), {} };
	}

	// タイルマップとの衝突判定
	const CollisionResult RectCollider::collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const TileMapCollider& targetCollider) const {
		return overlap_rect_vs_tile_map(
			objectCenterPos + offset - (size / 2) - targetCenterPos,
			size,
			targetCollider.get_layer_array(),
			targetCollider.get_tile_size()
		);
	}
}
