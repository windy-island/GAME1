#pragma once

#include "RectCollider.h"
#include "CircleCollider.h"
#include "PointCollider.h"
#include "TileMapCollider.hpp"

using namespace SGM2::plus::hitbox::collider;

namespace SGM2::plus::hitbox {

	class HitBoxComponent;

	class ColliderSet {

		friend class HitBoxComponent;

	private:
		vector<unique_ptr<Collider>> colliders;	// コライダーリスト
		bool visibleFlag;						// true の場合描画

	public:
		ColliderSet() : colliders(), visibleFlag(false) {}

		const vector<unique_ptr<Collider>>& get_colliders() const { return colliders; }

	public:
		// 矩形コライダーを追加
		void add_rect_collider(const Vec2I& offset, const Size& size) {
			colliders.emplace_back(new RectCollider(offset, size));
		}

		// 円コライダーを追加
		void add_circle_collider(const Vec2I& offset, const int32 r) {
			colliders.emplace_back(new CircleCollider(offset, r));
		}

		// 点コライダーを追加
		void add_point_collider(const Vec2I& offset) {
			colliders.emplace_back(new PointCollider(offset));
		}

		// タイルマップコライダーを作成
		void add_tile_map_collider(const vector<vector<uint32>> layer_, const Size tileSize_) {
			colliders.emplace_back(new TileMapCollider(layer_, tileSize_));
		}
		void add_tile_map_collider(const SimpleTileLayerData& data) {
			add_tile_map_collider(data.data, data.tileSize);
		}

		// ★★★ コライダーを削除

		// 特定の ColliderSet と接触しているかどうかを判定
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const ColliderSet& target) const {

			// 結果
			CollisionResult result;

			// ターゲットがタイルマップを含むかどうか
			// つまり CollisionResult のサブデータ欄を利用するかどうか
			// 利用しないなら衝突が分かった時点で判定を終了できる
			bool targetIncludeTileMap = false;
			for (const auto& targetCollider : target.get_colliders()) {
				if (targetCollider->get_collider_type() == ColliderType::TileMap) {
					targetIncludeTileMap = true;
				}
			}

			// 衝突したかどうか
			bool isHit = false;

			// 衝突判定
			for (const auto& collider : colliders) {
				if (isHit && !targetIncludeTileMap) break;

				for (const auto& targetCollider : target.get_colliders()) {
					if (isHit && !targetIncludeTileMap) break;

					auto ret = collider->collide_with(objectCenterPos, targetCenterPos, *targetCollider);

					if (isHit == false && ret.isOverlapping) {
						isHit = ret.isOverlapping;
						result.isOverlapping = ret.isOverlapping;
					}

					if (!ret.dataA.empty()) {
						result.dataA.insert(result.dataA.begin(), ret.dataA.begin(), ret.dataA.end());
					}
				}
			}

			return result;
		}

		void set_visible_flag(const bool flag) { visibleFlag = flag; }

		bool get_visible_flag() const { return visibleFlag; }

	private:
		// デバッグ描画
		void debug_draw(const DrawingProperties& dp, const Color& color) const {
			if (!visibleFlag) {
				return;
			}

			for (const auto& collider : colliders) {
				collider->debug_draw(dp, color);
			}
		}
	};
}
