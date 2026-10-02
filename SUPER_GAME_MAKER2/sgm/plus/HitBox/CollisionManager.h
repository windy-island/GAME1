#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"

namespace SGM2::plus::hitbox {

	class HitBoxComponent;

	// ★★★ // ★★★ // ★★★ <<< 注意 >>> ★★★ // ★★★ // ★★★ //
	// ● 1
	// 接触判定は最初に行われるべきなので、Priorityはできるだけ高く設定すべきだ
	// 接触状態に応じて処理分岐するオブジェクトは多いと思うので
	// ● 2
	// createしたhitboxはCollisionManagerに格納される
	// ただし、オブジェクト側にも所有権を持たせないと削除されてしまうの注意
	// ちゃんとコンポーネントとして持たせていればよい
	// というか、そうしなきゃだめ
	class CollisionManager : public Object {
	public:
		explicit CollisionManager(const int priority = INT_MAX) : Object(priority) {}

		shared_ptr<HitBoxComponent> create_hitbox(Object& obj_, const bool checkFlag_ = true, const uint32_t groupId_ = 0);

		// これを呼び出せば即座に当たり判定の計算を行える
		// 判定を行いながら位置を微調整したいときや、生成位置を調整したいときなど、
		// いろいろな場面で利用可能
		void recalc_collision(HitBoxComponent& hitBox) const;

		int32 get_hitboxes_num() const {
			int32 count = 0;
			for (const auto& v : hitboxes) {
				count += (int32)v.second.size();
			}
			return count;
		}

	private:
		unordered_map<int, vector<shared_ptr<HitBoxComponent>>> hitboxes;


		// 自動で全ヒットボックスの当たり判定を行う
		bool update() override;

		void calc_collision(HitBoxComponent& hitBox) const;
	};
}
