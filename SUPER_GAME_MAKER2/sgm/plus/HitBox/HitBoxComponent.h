#pragma once

#include "sgm.h"
#include "Position.hpp"
#include "Siv3Dplus.h"
#include "ColliderSet.hpp"

/*
* 判定用に図形を足していくわけだが、
* 中央（Positionコンポーネントの座標）からずれる場合はoffsetを指定する
*/

/*
* デバッグ描画にコンポーネントが備える draw は使用しない。
* draw関数 はコンポーネントを保持するオブジェクトからコールされるためである。
* デバッグ描画は CollisionManager から行う。
* こうすることで、判定をまとめて最前面に描画しやすくなる。
* 要はオブジェクト同士の前後関係に左右されない。
*						↓
* やっぱりコンポーネントでやる。
* カメラを介した描画位置の取得が面倒なので。
*/

namespace SGM2::plus::hitbox {

	class CollisionManager;

	// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	// 有効化されているコライダーセットが削除されるとまずい
	// 削除された場合はポインタにnullptrをセットするような仕組みを実装すること

	class HitBoxComponent : public Component {

		// ******************** 定数 ************************************************************************************************ //

		static constexpr Color DEFAULT_DEBUG_COLOR{ ColorF(0.0, 1.0, 0.0, 0.7) };





		// ******************** フレンド ******************************************************************************************** //

		friend class CollisionManager;





		// ******************** メンバ変数 ****************************************************************************************** //

	private:
		static uint32_t nextId;

		uint32_t id;
		uint32_t objectId;
		vector<uint32_t> targetIdList;						// 接触判定を行う相手のグループID（自身のグループIDはキーとしてManagerに記憶される）
	protected:
		vector<const HitBoxComponent*> collidingHitboxes;	// 接触しているHitBox
		vector<int32> subCollisionDataA;					// 特殊な衝突判定を行った場合に利用される領域。タイルマップとの判定で重なっているタイルIDを入れるなど。
		bool checkFlag;										// これがfalseだと、check_hitが呼ばれない
	private:
		vector<string> tags;
		CollisionManager& cm;
		unordered_map<string, ColliderSet> colliderSetDict;
		ColliderSet *activeColliderSet;
		Color debugColor;									// デバッグ描画の際に使用する色



	
		// ******************** 特殊関数 ******************************************************************************************** //

	private:
		HitBoxComponent(CollisionManager& cm_, Object& obj_, const bool checkFlag_ = true) :
			Component(),
			id(nextId),
			objectId(obj_.get_id()),
			targetIdList({ 0 }),
			collidingHitboxes(),
			subCollisionDataA(),
			checkFlag(checkFlag_),
			tags(),
			cm(cm_),
			colliderSetDict(),
			activeColliderSet(nullptr),
			debugColor(DEFAULT_DEBUG_COLOR) {
			++nextId;
		}

	public:
		virtual ~HitBoxComponent() {}	// これが private だと shared_ptr から管理できない





		// ******************** コンポーネント関数 ********************************************************************************** //

	private:
		void ready() override {
			//objPos = object->get_component_cshare<Position>();
			//if (!objPos)
			//	throw std::runtime_error("CollisionDetector : Positionコンポーネントがないです");
		}

		// デバッグ描画
		void draw(const DrawingProperties& dp) const override {
			if (!activeColliderSet) {
				return;
			}
			activeColliderSet->debug_draw(dp, debugColor);
		}





		// ******************** Setter ********************************************************************************************** //

	public:
		constexpr void add_tag(const string& tag) { tags.push_back(tag); }

		constexpr void reset_target_group_id(const vector<uint32_t>& list) {
			targetIdList = list;
			std::sort(targetIdList.begin(), targetIdList.end());
			targetIdList.erase(std::unique(targetIdList.begin(), targetIdList.end()), targetIdList.end());
		}

		void set_debug_color(const Color& color) { debugColor = color; }





		// ******************** Getter ********************************************************************************************** //

	public:
		// 指定したキーを持つコライダーセットを取得
		// 存在しないキーを渡すと例外が飛ぶ
		ColliderSet& get_collider_set(const string& key) {
			if (colliderSetDict.contains(key)) {
				return colliderSetDict.at(key);
			}
			else {
				throw std::runtime_error("存在しないコライダーセットを取得しようとしました。無効なキー指定です。");
			}
		}

		constexpr const vector<const HitBoxComponent*>& get_colliding_hitboxes() const { return collidingHitboxes; }

		constexpr const vector<int32>& get_sub_collision_dataA() const { return subCollisionDataA; }

		constexpr const vector<string>& get_tags() const { return tags; }

		const uint32_t get_object_id() const { return objectId; }

		const ColliderSet* get_active_collider_set() const { return activeColliderSet; }





		// ******************** Public メンバ関数 ********************************************************************************************** //

	public:

		// コライダーセットの追加
		// 空白や重複するキーを渡すと例外が飛ぶ
		// 追加したコライダーセットを自動で有効化する
		ColliderSet& add_collider_set(const string& key) {
			if (key.empty() || colliderSetDict.contains(key)) {
				throw std::runtime_error("コライダーセット追加時に無効なキーを指定しました。空白、または名前の重複です。");
			}
			colliderSetDict[key] = ColliderSet();
			activate(key);
			return colliderSetDict[key];
		}

		// タイルマップコライダーセットの追加
		// コライダーセットの追加とタイルマップコライダーの追加を一括で行う関数
		// 追加したコライダーセットを自動で有効化する
		void add_tile_map_collider_set(const vector<vector<uint32>> layer_, const Size tileSize_, const string& key = "tilemap") {
			if (key.empty() || colliderSetDict.contains(key)) {
				throw std::runtime_error("コライダーセット追加時に無効なキーを指定しました。空白、または名前の重複です。");
			}
			ColliderSet cset;
			cset.add_tile_map_collider(layer_, tileSize_);
			colliderSetDict[key] = std::move(cset);
			activate(key);
		}
		void add_tile_map_collider_set(const SimpleTileLayerData& data, const string& key = "tilemap") {
			add_tile_map_collider_set(data.data, data.tileSize, key);
		}

		// 指定されたコライダーセットを有効化する
		// 存在しないキーを指定した場合、例外を飛ばす
		// 当たり判定の実行フラグを立てる役割もあり、
		// 引数なしで呼び出した場合はフラグ立てのみを行う
		void activate(const string& colliderSetKey = "") {

			if (colliderSetKey != "") {
				// 指定されたキーでコライダーセットを有効化する
				if (colliderSetDict.contains(colliderSetKey)) {
					activeColliderSet = &colliderSetDict.at(colliderSetKey);
				}
				else {
					string errMessage = std::format("存在しないコライダーセットを有効化しようとしました！指定されたキーは使用されていません。[ {} ]", colliderSetKey);
					throw std::runtime_error(errMessage);
				}
			}
			checkFlag = true;
		}

		// ベクトル実体を返すので、ちょっとコピーの負荷があるかも
		// ポインタで返した方いいかな？
		// でも、動的確保の方が遅くなる可能性もある？
		constexpr const vector<const HitBoxComponent*> get_colliding_hitboxes_by_tag(const string& tag) const {
			vector<const HitBoxComponent*> hitBoxes;
			std::copy_if(collidingHitboxes.begin(), collidingHitboxes.end(), std::back_inserter(hitBoxes), [&tag](const HitBoxComponent* hitBox) {
				return std::ranges::contains(hitBox->get_tags(), tag);
				});
			return hitBoxes;
		}

		// 当たり判定を無効化
		void deactivate() { checkFlag = false; }

		constexpr bool is_hit() const { return collidingHitboxes.empty() ? false : true; }

		constexpr vector<string> get_others_tag() const {
			vector<string> tags;
			for (const auto& other : collidingHitboxes)
				tags.insert(tags.end(), other->tags.begin(), other->tags.end());
			std::sort(tags.begin(), tags.end());
			tags.erase(std::unique(tags.begin(), tags.end()), tags.end());
			return tags;
		}

		constexpr bool others_tag_contain(const string& tag_) const {
			for (const auto& other : collidingHitboxes)
				for (const auto& tag : other->tags)
					if (tag_ == tag)
						return true;
			return false;
		}

		// 多分当たり判定の再計算をしたいときに呼び出す
		void calc_collision();

		// 衝突相手の数を取得
		int32 get_colliding_hitboxes_num() const { return (int32)collidingHitboxes.size(); }




		// ******************** Private メンバ関数 ********************************************************************************************* //
	private:

		// 対象の Hitbox と衝突しているかどうかを調べる
		// ユーザーが間違ってコールしないよう、privateにする
		// 呼べるようにしといてもいいのかな？
		const CollisionResult collide_with(const HitBoxComponent& target) {
			if (activeColliderSet != nullptr && target.get_active_collider_set() != nullptr) {
				return activeColliderSet->collide_with(object->get_global_posI_xy(), target.object->get_global_posI_xy(), *target.get_active_collider_set());
			}
			return { false, {} };
		}
	};
}
