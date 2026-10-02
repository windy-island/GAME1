#pragma once

#include "sgm.h"

namespace SGM2::plus::hitbox::collider {

	struct CollisionResult;

	// 矩形と矩形の重なり判定
	const bool overlap_rect_vs_rect(const Vec2I& posUL_A, const Size& size_A, const Vec2I& posUL_B, const Size& size_B);

	// 円と円の重なり判定
	const bool overlap_circle_vs_circle(const Vec2I& centerA, const int32 rA, const Vec2I& centerB, const int32 rB);

	// 円と矩形の重なり判定
	const bool overlap_circle_vs_rect(const Vec2I& center, const int32 r, const Vec2I& rectUL, const Size& rectSize);

	// 点と点の重なり判定
	const bool overlap_point_vs_point(const Vec2I& posA, const Vec2I& posB);

	// 点と矩形の重なり判定
	const bool overlap_point_vs_rect(const Vec2I& pointPos, const Vec2I& rectUL, const Size& rectSize);

	// 点と円の重なり判定
	const bool overlap_point_vs_circle(const Vec2I& pointPos, const Vec2I& center, const int32 r);

	// 点とタイルマップの重なり判定
	const CollisionResult overlap_point_vs_tile_map(const Vec2I& pointPos, const vector<vector<uint32>> mapLayer, const Size tileSize);

	// 矩形とタイルマップの重なり判定
	const CollisionResult overlap_rect_vs_tile_map(const Vec2I& rectUL, const Size& rectSize, const vector<vector<uint32>> mapLayer, const Size tileSize);

	// 円とタイルマップの重なり判定 ★★★★★★★★★★
}
