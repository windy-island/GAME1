#pragma once

#include "Collider.hpp"

namespace SGM2::plus::hitbox::collider {

	class RectCollider;
	class CircleCollider;
	class PointCollider;

	// TileMapColliderを作成するのに必要なデータ
	struct SimpleTileLayerData {
		vector<vector<uint32>> data;
		Size tileSize;
	};

	class TileMapCollider : public Collider {

		const vector<vector<uint32>> layer;
		const Size tileSize;

	public:
		TileMapCollider(const vector<vector<uint32>> layer_, const Size tileSize_) :
			Collider(ColliderType::TileMap),
			layer(layer_),
			tileSize(tileSize_) {
		}

		TileMapCollider(const SimpleTileLayerData& data) : TileMapCollider(data.data, data.tileSize) {}

	public:
		const vector<vector<uint32>>& get_layer_array() const { return layer; }

		const Size& get_tile_size() const { return tileSize; }

	private:

		// 以下の判定関数はいずれ削除されます。★★★★★★★★★★★★
		// タイルマップが他と判定を行う必要はないため。

		// 矩形との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const RectCollider& targetCollider) const override { return { false, {} }; }

		// 円との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const CircleCollider& targetCollider) const override { return { false, {} }; }

		// 点との衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const PointCollider& targetCollider) const override { return { false, {} }; }

		// タイルマップとの衝突判定を行う
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const TileMapCollider& targetCollider) const override { return { false, {} }; }

		// デバッグ描画はなし
		virtual void debug_draw(const DrawingProperties& dp, const Color& color) const {
			return;
		}
	};
}
