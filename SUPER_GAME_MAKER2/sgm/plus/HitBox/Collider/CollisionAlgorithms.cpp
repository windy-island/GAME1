#include "CollisionAlgorithms.h"
#include "Collider.hpp"

namespace SGM2::plus::hitbox::collider {

	/*
	* 見た目と判定を同期させるため int で計算している。
	* あくまで重なり判定であり、接するのみの場合は false を返す。
	* つまり不等号にイコールは含めない。
	*/

	// 矩形と矩形の重なり判定
	const bool overlap_rect_vs_rect(const Vec2I& posUL_A, const Size& size_A, const Vec2I& posUL_B, const Size& size_B) {
		return posUL_A.x < posUL_B.x + size_B.x
			&& posUL_A.x + size_A.x > posUL_B.x
			&& posUL_A.y < posUL_B.y + size_B.y
			&& posUL_A.y + size_A.y > posUL_B.y;
	}

	// 円と円の重なり判定
	const bool overlap_circle_vs_circle(const Vec2I& centerA, const int32 rA, const Vec2I& centerB, const int32 rB) {
		return (centerA.x - centerB.x) * (centerA.x - centerB.x) + (centerA.y - centerB.y) * (centerA.y - centerB.y) < (rA + rB) * (rA + rB);
	}

	// 円と矩形の重なり判定
	const bool overlap_circle_vs_rect(const Vec2I& center, const int32 r, const Vec2I& rectUL, const Size& rectSize) {
		const auto rectDR = rectUL + rectSize;

		return (center.x > rectUL.x && center.x < rectDR.x && center.y > rectUL.y - r && center.y < rectDR.y + r)
			|| (center.x > rectUL.x - r && center.x < rectDR.x + r && center.y > rectUL.y && center.y < rectDR.y)
			|| ((center.x - rectUL.x) * (center.x - rectUL.x) + (center.y - rectUL.y) * (center.y - rectUL.y) < r * r)
			|| ((center.x - (rectUL.x + rectSize.x)) * (center.x - (rectUL.x + rectSize.x)) + (center.y - rectUL.y) * (center.y - rectUL.y) < r * r)
			|| ((center.x - rectUL.x) * (center.x - rectUL.x) + (center.y - (rectUL.y + rectSize.y)) * (center.y - (rectUL.y + rectSize.y)) < r * r)
			|| ((center.x - rectDR.x) * (center.x - rectDR.x) + (center.y - rectDR.y) * (center.y - rectDR.y) < r * r);
	}

	// 点と点の重なり判定
	const bool overlap_point_vs_point(const Vec2I& posA, const Vec2I& posB) {
		return posA == posB;
	}

	// 点と矩形の重なり判定
	const bool overlap_point_vs_rect(const Vec2I& pointPos, const Vec2I& rectUL, const Size& rectSize) {
		return pointPos.x > rectUL.x && pointPos.x < rectUL.x + rectSize.x && pointPos.y > rectUL.y && pointPos.y < rectUL.y + rectSize.y;
	}

	// 点と円の重なり判定
	const bool overlap_point_vs_circle(const Vec2I& pointPos, const Vec2I& center, const int32 r) {
		return (pointPos.x - center.x) * (pointPos.x - center.x) + (pointPos.y - center.y) * (pointPos.y - center.y) < r * r;
	}

	// 点とタイルマップの重なり判定
	const CollisionResult overlap_point_vs_tile_map(const Vec2I& pointPos, const vector<vector<uint32>> mapLayer, const Size tileSize) {
		const auto idxX = pointPos.x / tileSize.x;
		const auto idxY = pointPos.y / tileSize.y;
		if (!mapLayer.empty() && idxY >= 0 && idxY < mapLayer.size() && idxX >= 0 && idxX < mapLayer.at(0).size()) {
			return { true, { (int)mapLayer.at(idxY).at(idxX) } };
		}
		return { false, {} };
	}

	// 矩形とタイルマップの重なり判定
	const CollisionResult overlap_rect_vs_tile_map(const Vec2I& rectUL, const Size& rectSize, const vector<vector<uint32>> mapLayer, const Size tileSize) {
		const Vec2I indexUL = rectUL / tileSize;
		const Vec2I indexDR = (rectUL + rectSize) / tileSize;
		CollisionResult result;

		for (int y = indexUL.y; y <= indexDR.y; ++y) {
			for (int x = indexUL.x; x <= indexDR.x; ++x) {
				if (!mapLayer.empty() && y >= 0 && y < mapLayer.size() && x >= 0 && x < mapLayer.at(0).size()) {
					result.isOverlapping = true;
					result.dataA.push_back((int)mapLayer.at(y).at(x));
				}
			}
		}
		return result;
	}
}
