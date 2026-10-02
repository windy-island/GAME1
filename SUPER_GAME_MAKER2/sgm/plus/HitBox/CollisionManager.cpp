
#include "CollisionManager.h"
#include "HitBoxComponent.h"

namespace SGM2::plus::hitbox {

	shared_ptr<HitBoxComponent> CollisionManager::create_hitbox(Object& obj_, const bool checkFlag_, const uint32_t groupId_ ) {
		shared_ptr<HitBoxComponent> hitbox(new HitBoxComponent(*this, obj_, checkFlag_));
		hitboxes[groupId_].push_back(hitbox);
		return hitbox;
	}

	// 再計算時にはHitBoxを持つオブジェクトが削除されている可能性がある
	// 削除判定を行いつつ処理
	// updateからコールするcalc_collisionは別にあるので注意
	void CollisionManager::recalc_collision(HitBoxComponent& hitBox) const {

		// リストの初期化
		hitBox.collidingHitboxes.clear();
		hitBox.subCollisionDataA.clear();

		// 判定対象グループを走査して、ヒットしているかを調べる
		for (const auto& targetId : hitBox.targetIdList) {
			if (hitboxes.contains(targetId))
				for (auto& target : hitboxes.at(targetId))
					if (target.use_count() > 1) {
						const auto ret = hitBox.collide_with(*target);
						if (ret.isOverlapping) {
							hitBox.collidingHitboxes.push_back(target.get());

							// サブデータがあれば追加
							if (!ret.dataA.empty()) {
								hitBox.subCollisionDataA.insert(hitBox.subCollisionDataA.begin(), ret.dataA.begin(), ret.dataA.end());
							}
						}
					}
		}

		// リストから自分を除外する
		for (int i = (int)hitBox.collidingHitboxes.size() - 1; i >= 0; --i) {
			auto& hitList = hitBox.collidingHitboxes;
			const auto& other = hitList.at(i);
			if (hitBox.id == other->id) {
				hitList.erase(hitList.begin() + i);
				break;
			}
		}
	}

	// 同じ判定を2回ずつやっているのは明らかに無駄である
	// 遅そうならその辺の改善もしたい
	bool CollisionManager::update() {

		// 所有者のいないヒットボックスを削除
		for (auto& pair : hitboxes) {
			for (int i = (int)pair.second.size() - 1; i >= 0; --i) {
				auto& hitboxes = pair.second;
				if (hitboxes.at(i).use_count() <= 1)
					hitboxes.erase(hitboxes.begin() + i);
			}
		}

		// 各グループの配列を取り出す
		for (auto& pair : hitboxes) {

			// 配列内の各要素を取得
			for (auto& hitbox : pair.second) {
				if (!hitbox->checkFlag)
					break;
				calc_collision(*hitbox);
			}
		}

		return true;
	}

	void CollisionManager::calc_collision(HitBoxComponent& hitBox) const {

		// リストの初期化
		hitBox.collidingHitboxes.clear();
		hitBox.subCollisionDataA.clear();

		// 判定対象グループを走査して、ヒットしているかを調べる
		for (const auto& targetId : hitBox.targetIdList) {

			// 対象のグループに属する全ての ヒットボックス を走査
			if (hitboxes.contains(targetId)) {
				for (auto& other : hitboxes.at(targetId)) {

					// 衝突していたらリストに追加
					const auto ret = hitBox.collide_with(*other);
					if (ret.isOverlapping) {
						hitBox.collidingHitboxes.push_back(other.get());

						// サブデータがあれば追加
						if (!ret.dataA.empty()) {
							hitBox.subCollisionDataA.insert(hitBox.subCollisionDataA.begin(), ret.dataA.begin(), ret.dataA.end());
						}
					}
				}
			}
		}

		// リストから自分を除外する
		for (int i = (int)hitBox.collidingHitboxes.size() - 1; i >= 0; --i) {
			auto& hitList = hitBox.collidingHitboxes;
			const auto& other = hitList.at(i);
			if (hitBox.id == other->id) {
				hitList.erase(hitList.begin() + i);
				break;
			}
		}
	}
}
