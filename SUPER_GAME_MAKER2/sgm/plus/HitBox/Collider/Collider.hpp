#pragma once

#include "sgm.h"

namespace SGM2::plus::hitbox {
	class ColliderSet;
}

namespace SGM2::plus::hitbox::collider {

	class RectCollider;
	class CircleCollider;
	class PointCollider;
	class TileMapCollider;

	enum class ColliderType {
		Rect,
		Circle,
		Point,
		TileMap
	};

	// 衝突判定の結果
	// vectorは空であればヒープを使用しないらしいので、生成時の処理コストはそこまで大きくないと思われる。
	struct CollisionResult {

		// 衝突しているかどうか
		bool isOverlapping = false;

		// サブデータ（タイルマップとの判定等で利用）
		vector<int32> dataA;
	};

	//// タイルマップとの衝突判定結果
	//struct CollsionResultVsTileMap : public CollisionResult {
	//	vector<int32> tileIdList;
	//};

	class Collider {

		friend class SGM2::plus::hitbox::ColliderSet;

		// ***************** // メンバ変数 // *************************************************************************************** //

	protected:
		// コライダーの種類を表す（rectなど）。
		// 型の判定にダイナミックキャストを使うと遅くなるため、
		// この変数で型を判別する。
		const ColliderType type;





		// ***************** // 特殊関数 // ***************************************************************************************** //

	protected:
		Collider(const ColliderType type_) : type(type_) {}

	public:
		~Collider() = default;





		// ***************** // Getter // ******************************************************************************************* //

	public:
		const ColliderType get_collider_type() const { return type; }





		// ***************** // 純粋仮想関数 // ************************************************************************************* //

	private:
		// 矩形との衝突判定
		virtual const CollisionResult collide_with(const Vec2I& centerPos, const Vec2I& targetCenterPos, const RectCollider& target) const = 0;

		// 円との衝突判定
		virtual const CollisionResult collide_with(const Vec2I& centerPos, const Vec2I& targetCenterPos, const CircleCollider& target) const = 0;

		// 点との衝突判定
		virtual const CollisionResult collide_with(const Vec2I& centerPos, const Vec2I& targetCenterPos, const PointCollider& target) const = 0;

		// タイルマップとの衝突判定
		virtual const CollisionResult collide_with(const Vec2I& centerPos, const Vec2I& targetCenterPos, const TileMapCollider& target) const = 0;

		// デバッグ描画
		virtual void debug_draw(const DrawingProperties& dp, const Color& color) const = 0;





		// ***************** // Public メンバ関数 // ******************************************************************************** //

	public:
		// 衝突判定
		// 対象の形状によって処理を分岐
		const CollisionResult collide_with(const Vec2I& objectCenterPos, const Vec2I& targetCenterPos, const Collider& targetCollider) const {

			switch (targetCollider.get_collider_type()) {

			case ColliderType::Rect:
				return collide_with(objectCenterPos, targetCenterPos, (const RectCollider&)targetCollider);
				break;

			case ColliderType::Circle:
				return collide_with(objectCenterPos, targetCenterPos, (const CircleCollider&)targetCollider);
				break;

			case ColliderType::Point:
				return collide_with(objectCenterPos, targetCenterPos, (const PointCollider&)targetCollider);
				break;

			case ColliderType::TileMap:
				return collide_with(objectCenterPos, targetCenterPos, (const TileMapCollider&)targetCollider);
				break;

			default:
				throw std::runtime_error("不明なColliderとの衝突判定を行おうとしています。");
				break;
			}
		}

		// 引数の違いによるオーバーロード
		const CollisionResult collide_with(const Vec3& centerPos, const Vec3& targetCenterPos, const Collider& targetCollider) const {
			return collide_with(centerPos.xy().asPoint(), targetCenterPos.xy().asPoint(), targetCollider);
		}
		const CollisionResult collide_with(const Vec3I& centerPos, const Vec3I& targetCenterPos, const Collider& targetCollider) const {
			return collide_with(Vec2I{ centerPos.x, centerPos.y }, Vec2I{ targetCenterPos.x, targetCenterPos.y }, targetCollider);
		}
		const CollisionResult collide_with(const Vec2& centerPos, const Vec2& targetCenterPos, const Collider& targetCollider) const {
			return collide_with(centerPos.asPoint(), targetCenterPos.asPoint(), targetCollider);
		}
	};
}
