/*****************************************************************//**
 * @file   GameState.h
 * @brief  
 * 
 * @date   January 2024
 *********************************************************************/
#pragma once

#include "std.h"
#include "Siv3Dplus.h"
#include "Object.h"
#include "DrawingManager.hpp"

namespace SGM2 {

	class GameStateManager;
	//class Camera;

	/// @cond
	struct ConfigGS {
		//bool performZSoortEveryTime = false;	// 子オブジェクトは毎フレームソートされない
		bool tmp = false;	// 現在使用していません
	};
	/// @endcond

	/// @cond
	struct FlagGS {
		bool isActive = true;
		bool objectIsAdded = false;
		bool isInitialized = false;
	};
	/// @endcond

	/**
	 * @brief
	 * GS: ゲームステートを作成する際の基底クラス
	 * 
	 * @par 詳細
	 * 「Object」や「Component」のページも参照すること。
	 * <br><br>
	 * ゲームステートとは、一般的にシーンと呼ばれているものに相当する機能だ。
	 * GSにオブジェクトを追加することでワールドを作り、カメラとスクリーンを追加してワールドを描画する。
	 * <br>
	 * 状態遷移システムもあるが、この辺は「GameStateManager」の方に説明があるだろう。
	 * 
	 * @note
	 * カメラやスクリーンを取得する関数があってもいいかな？
	 * add時に取れるから、いらないっちゃいらないんだけど。
	 */
	class GameState {

		friend class GameStateManager;
		friend class Object;

	public:
		static constexpr int INVALID_ID = 0;
		static constexpr int FIRST_ID = 1;

		//■//■//■//■//■// 特殊関数 //■//■//■//■//■//

		/**
		 * @brief コンストラクタ
		 */
		GameState() :
			id(INVALID_ID),
			config(),
			flag(),
			objects(std::make_unique<vector<shared_ptr<Object>>>()),
			dm(std::make_unique<DrawingManager>()),
			effects(std::make_unique<Effect>()) {
		}

		GameState(GameState&& r) noexcept :
			id(r.id),
			config(r.config),
			flag(r.flag),
			objects(std::move(r.objects)),
			dm(std::move(dm)),
			effects(std::move(r.effects)) {
			r.objects.reset();
			r.effects.reset();
		}

		virtual ~GameState() = default;



		//■//■//■//■//■// オブジェクト操作 //■//■//■//■//■//

		// 自身にidがセットされるのは遷移するとき
		// よって、コンストラクタからの add_object では無効なIDをセットすることになってしまう
		// 対策として、GameState へのIDセット時に、オブジェクトのIDも指定し直している
		//
		// ↑オブジェクトにGSのポインタを持たせたので、現在はIDを渡していません

		/**
		 * @brief オブジェクトを追加する
		 *
		 * @par 詳細
		 * 一応readyからコールすべきだが、コンストラクタからコールしても動くように作ってあるらしい。
		 * <br>
		 * 実装を覚えてないのであれだが、GSへのIDセット時にオブジェクトのIDも指定し直すようだ。
		 * コンストラクタ内では当然GSMの管理外であるためIDがないが、先述の仕様によりなんとかなっているそう。
		 * <br><br>
		 * レイヤー番号も指定できる。レイヤーの小さいものほど手前に描画されるが、UIや背景等で使うぐらいだろう。
		 * 前後関係の制御はカメラのソート関数でやるのが基本なので、そっちのページも参照すること。
		 *
		 * @param obj 追加するオブジェクト
		 * @param layerNo オブジェクトを配置する描画レイヤー
		 * @return オブジェクトのポインタ
		 *
		 * @note
		 * shared_ptrを返すのはどうなのか。
		 * <br>
		 * 安全面を考慮するならweak_ptrの方が絶対にいいが、仕様を熟知しているのならshared_ptrの方が楽なケースもある。
		 * この辺どうするのがいいでしょうねぇ？
		 */
		template<typename T>
		shared_ptr<T> add_object(shared_ptr<T> obj, const int32 layerNo = 0) {
			if (obj != nullptr) {
				//obj->set_game_state_id(id);
				obj->gameState = this;
				obj->init_layer_no(layerNo);
				objects->push_back(obj);
				flag.objectIsAdded = true;

				// DMにもオブジェクトを渡す
				dm->add_object(obj, layerNo);
				return obj;
			}
			return nullptr;
		}

		template<typename T>
		shared_ptr<T> add_object(T* obj, const int32 layerNo = 0) {
			return add_object(shared_ptr<T>(obj), layerNo);
		}

		// 参照先のオブジェクトが破棄される可能性を考えると、weak_ptrの方がよい
		// sharedだと所有者が増え続けて寿命の管理が煩雑になる

		/**
		 * @brief ID指定でオブジェクトを取得
		 *
		 * @par 詳細
		 * 失敗すると無効なweak_ptrが返る。
		 *
		 * @param id_ オブジェクトのID
		 * @return  オブジェクトのポインタ
		 */
		weak_ptr<Object> get_object_by_id(const int id_) {
			auto it = std::find_if(objects->begin(), objects->end(), [id_](const auto& v) {
				return v->get_id() == id_;
			});
			if (it != objects->end())
				return *it;
			return weak_ptr<Object>();
		}

		/**
		 * @brief 型指定でオブジェクトを取得
		 *
		 * @par 詳細
		 * これ系の関数はダイナミックキャストが入る分若干遅い。
		 * あまり多用すべきではないだろう。
		 * <br>
		 * 最初にマッチしたやつを1つだけ返す。
		 *
		 * @return オブジェクトのポインタ
		 *
		 * @note
		 * 複数取るやつもほしい。
		 */
		template<typename T>
		weak_ptr<T> get_object_by_type() {
			for (auto& object : *objects) {
				//if (typeid(T).name() == object->get_type())
				//	return std::static_pointer_cast<T>(object);
				if (const auto& ptr = std::dynamic_pointer_cast<T>(object))
					return ptr;
				auto ret = object->get_child_by_type<T>();
				if (!ret.expired())
					return ret;
			}
			return weak_ptr<T>();
		}

		/**
		 * @brief 型指定でオブジェクトを取得
		 *
		 * @par 詳細
		 * 孫の探索は行わず、子の中からのみ探索する。
		 * あとは「get_object_by_type」と同じ。
		 *
		 * @return オブジェクトのポインタ
		 */
		template<typename T>
		weak_ptr<T> get_object_from_top_by_type() {
			for (auto& object : *objects) {
				if (const auto& ptr = std::dynamic_pointer_cast<T>(object))
					return ptr;
				//if (typeid(T).name() == object->get_type())
				//	return std::static_pointer_cast<T>(object);
			}
			return weak_ptr<T>();
		}

		/**
		 * @brief カメラを追加する
		 *
		 * @par 詳細
		 * これとスクリーンを追加しないと描画ができない。
		 * 一応GSのdrawは動くが。
		 *
		 * @param camera カメラ
		 * @return カメラのポインタ
		 */
		template<typename T>
		shared_ptr<T> add_camera(shared_ptr<T> camera, const int32 layerNo = 0) {
			add_object(camera, layerNo);
			return dm->add_camera(camera);
		}

		template<typename T>
		shared_ptr<T> add_camera(T* camera, const int32 layerNo = 0) {
			return add_camera(shared_ptr<T>(camera), layerNo);
		}

		/**
		 * @brief スクリーンを追加する
		 *
		 * @par 詳細
		 * これとスクリーンを追加しないと描画ができない。
		 * 一応GSのdrawは動くが。
		 *
		 * @param screen スクリーン
		 * @return スクリーンのポインタ
		 */
		shared_ptr<Screen> add_screen(shared_ptr<Screen> screen) {
			return dm->add_screen(screen);
		}

		shared_ptr<Screen> add_screen(Screen* screen) {
			return add_screen(shared_ptr<Screen>(screen));
		}

		/**
		 * @brief カメラにソートを実行させる
		 */
		void reserve_sort() { dm->reserve_sort(); }


		//■//■//■//■//■// ゲッター //■//■//■//■//■//

		/**
		 * @brief IDを取得
		 */
		int get_id() const { return id; }

		string get_type_name() const { return std::string(typeid(*this).name()); }

		/**
		* @brief 活性状態を取得
		*/
		bool is_active() const { return flag.isActive; }

		/**
		* @brief オブジェクトの数を取得
		*
		* @par 詳細
		* 子オブジェクトまではカウントしない。
		*
		* @note
		* 子オブジェクトまでカウントするやつもほしい。
		*/
		int get_objects_num() const { return (int)objects->size(); }


		/**
		 * @brief 描画レイヤーの数を取得
		 */
		int32 get_layer_count() const { return dm->get_layer_count(); }

	private:
		//■//■//■//■//■// 仮想関数 //■//■//■//■//■//

		/**
		 * @brief 準備関数
		 *
		 * @par 詳細
		 * 初回更新時にコールされる。
		 * <br><br>
		 * コンストラクタと違って、この関数がコールされる時点ではGSMの管理下にある。
		 * <br><br>
		 * 実行タイミングの詳細は「Object」や「Component」のページを参照。
		 */
		virtual void ready() {}

		/**
		 * @brief 更新関数
		 *
		 * @par 詳細
		 * 毎フレームコールされる。
		 */
		virtual void update() {}

		/**
		 * @brief 描画関数
		 * 
		 * @par 詳細
		 * この関数はカメラやスクリーンの影響を受けない。普通にクライアント座標値を指定して描画するのみ。
		 */
		virtual void draw() const {}



		//■//■//■//■//■// 更新関数 //■//■//■//■//■//

		void trigger_update();



		//■//■//■//■//■// 活性状態操作 //■//■//■//■//■//

		void activate() {
			flag.isActive = true;
		}

		void deactivate() {
			flag.isActive = false;
		}



		//■//■//■//■//■// セッター //■//■//■//■//■//

		void set_id(const int id_) {
			id = id_;
			//for (auto& obj : *objects)
			//	obj->set_game_state_id(id_);
		}

		// オブジェクトが子オブジェクトを追加する際、
		// この関数を使ってカメラにも子を渡す
		// 内部で使う用なので、ユーザーは使用しない
		void set_child_to_camera(weak_ptr<Object> child, const int32 layerNo = 0) {
			dm->add_object(child, layerNo);
		}

		uint32 id;				// 1はじまりとする
		ConfigGS config;
		FlagGS flag;
		unique_ptr<vector<shared_ptr<Object>>> objects;		// ムーブできるようにポインタ化しておく
		unique_ptr<DrawingManager> dm;
		// objects は priority でソート

	public:
		unique_ptr<Effect> effects;

#ifdef _DEBUG
	public:
		vector<shared_ptr<Object>>* DEBUG_get_object_container() {
			return objects.get();
		}
#endif
	};
}

#include "Object_impl.h"
