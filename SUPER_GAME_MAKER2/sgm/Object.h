#pragma once

#include "std.h"
#include "Component.hpp"
//#include "GameState.h"
#include "Position.hpp"
#include "DrawType.hpp"

namespace SGM2 {

	class GameState;
	class CameraBase;
	struct DrawingProperties;

	// 描画時にZソートする都合上、Postionコンポーネントのみデフォで追加済み
	/**
	 * @brief 
	 * Obj: ゲームオブジェクトを作成する際の基底クラス
	 * 
	 * @par 詳細
	 * <br>
	 * 「Compoent」のページも参照すること。
	 * <br><br>
	 * 「オブジェクト」はSGM2のゲーム開発において重要な立ち位置を占める。
	 * update関数をオーバーライドすることで挙動を決め、
	 * draw関数をオーバーライドすることで見た目を決める。
	 * <br><br>
	 * コンポーネントの追加や子オブジェクトの追加も可能。
	 * <br><br>
	 * 文字列のタグを複数持たせられる。
	 * <br><br>
	 * 子オブジェクトを持たせることで階層構造を作ることもできる。
	 * <br>
	 * 子オブジェクトの位置はローカル座標値となり、親からの相対的な値となる。
	 * 
	 * @important
	 * **★ 実行順序**<br>
	 * 自身よりもコンポーネントが先に更新されるのは「コンポーネント」のページに書いた通りである。
	 * しかし、それよりも先に子オブジェクトが更新される。具体的な順番は以下。
	 * <br><br>
	 * 1 子オブジェクト [ ready & update ]<br>
	 * 2 コンポーネント [ ready & update ]<br>
	 * 3 オブジェクト [ ready & update ]<br>
	 * <br>
	 * ゲームステートも込みの実行順序は「Component」のページに説明がある。
	 * 
	 * @note
	 * タグの活用例についても記述したい。
	 * <br><br>
	 * 特定のタグを持つかどうか返す関数などがあってもいいだろう。
	 * <br><br>
	 * 型名を返す関数が2つもあるが、おそらくいらないので削除を検討すべき。
	 */
	class Object {

		// ******************** フレンド ******************************************************************************************** //
		
		// メンバ関数まで指定したかったが、ヘッダと実装を分けるのが面倒なのもあってクラス指定です
		// 多分、クラスの定義が見えないとメンバ関数のfriend指定ができない
		friend class GameState;
		friend class CameraBase;





		// ******************** 内部クラス ******************************************************************************************* //

	public:
		/**
		 * @brief 
		 * オブジェクトの設定
		 */
		struct Config {
			/**
			 * @brief DPが必要かどうか
			 * 
			 * @par 詳細
			 * カメラがDPを計算して渡してくるが、場合によってはこれらを使わないケースもあるだろう。
			 * そういう場合はこの設定をtrueにする。
			 * <br>
			 * 計算を省略するようになり、処理コストが下がる。
			 * <br>
			 * カメラやスクリーンなど、計算の不要なものは有効な値が渡される。
			 */
			bool needsDrawingProperties = true;

			/**
			 * @brief 必ず描画させる
			 * 
			 * @par 詳細
			 * カメラにはビューポート外の描画を省略する機能があるが、この関数をコールすると必ずdraw関数がコールされる。
			 */
			bool mustDraw = false;

			/**
			 * @brief 描画タイプ
			 * 
			 * @par 詳細
			 * 図形か画像かを選ぶ。<br>
			 * カメラにシェーダーをセットする場合、
			 * これを正しく選択しないと描画がおかしくなる。
			 * カメラシェーダーなしなら気にしなくてもよい。
			 * <br><br>
			 * あと、コンポーネントの描画もオブジェクトの描画タイプと同じ扱いになる。
			 * 
			 * @note
			 * 図形と画像の片方しか描画できないのは少々面倒。
			 * いい対処方法を考えないと。
			 */
			DrawType drawType = DrawType::Texture;
		};





		// ******************** 定数 ************************************************************************************************ //

	public:
		static constexpr int FIRST_OBJECT_ID = 1;
		//static constexpr int INVALID_GAME_STATE_ID = 0;





		// ******************** メンバ変数 ****************************************************************************************** //

	private:
		static int nextId;

	protected:
		GameState* gameState;

		// shared_ptrにすると親が消せなくなる
		// weak_ptrはthisから取れない
		// ゆえに生ポインタ
		Object* parent;
	private:
		const int id;
		//int gameSstateId;
		const int priority;
		bool isActive;
		bool isSorted;
		bool componentIsAdded;
		bool isInitialized;
		vector<string> tags;
		vector<shared_ptr<Component>> components;

		vector<shared_ptr<Object>> children;
		bool childrenIsAdded;
		std::optional<int32> layerNo;
	protected:
		Vec3& pos;
		Config config;





		// ******************** 特殊関数 ******************************************************************************************** //

		/**
		 * @brief 
		 * コンストラクタ
		 * 
		 * @par 詳細
		 * Positionコンポーネントの初期値と優先度を指定できる。
		 * <br><br>
		 * Positionはワールド座標値として扱かわれるはずだが、
		 * この辺はカメラの設計によるため絶対じゃない。
		 * <br><br>
		 * 優先度については「Component」のページを参照。
		 * <br><br>
		 * 戻り値として false を返すと自身が破棄される。
		 * 基本的には true を返し続けていればよい。
		 * <br><br>
		 * 第一引数を優先度にしたオーバーロード版があるため、
		 * 優先度だけ指定して生成することも可能。
		 * 
		 * @param pos_
		 * ワールド座標値
		 * 
		 * @param priority_
		 * 優先度
		 */
	public:
		Object(const Vec3& pos_ = { 0, 0, 0 }, const int32 priority_ = 0) noexcept :
			gameState(nullptr),
			parent(nullptr),
			id(nextId),
			//gameSstateId(INVALID_GAME_STATE_ID),
			priority(priority_),
			isActive(true),
			isSorted(false),
			componentIsAdded(false),
			isInitialized(false),
			tags(),
			components(),
			children(),
			childrenIsAdded(true),
			layerNo(std::nullopt),
			pos(add_component(new Position(pos_))->pos),
			config() {
			++nextId;
		}

		explicit Object(const int priority_, const Vec3& pos_ = { 0, 0, 0 }) noexcept :
			Object(pos_, priority_) {}

		virtual ~Object() = default;





		// ******************** コンポーネント操作関数 ****************************************************************************** //

		//void add_component(shared_ptr<Component> com) {
		//	components.push_back(com);
		//	componentIsAdded = true;
		//}

		//void add_component(Component* com) {
		//	add_component(shared_ptr<Component>(com));
		//}

		//template<typename T>
		//shared_ptr<T> add_component(shared_ptr<T> com) {
		//	components.push_back(com);
		//	componentIsAdded = true;
		//	return com;
		//}

		/**
		 * @brief 
		 * コンポーネントを追加
		 * 
		 * @par 詳細
		 * 引数に指定したコンポーネントを追加する。
		 * 指定するコンポーネントは shared_ptr か生ポインタだ。
		 * <br><br>
		 * 追加したコンポーネントはすぐに始動せず、次のフレームから動き出す。
		 * この辺の挙動は「Component」のページを参照。
		 * <br><br>
		 * コンポーネントの shared_ptr が戻り値として取得できる。
		 * get_component を使わずとも、ここでコンポーネントを保持することが可能。
		 * <br><br>
		 * nullptr を渡した場合、追加はせずに nullptr を返す。
		 * 
		 * @param com
		 * 追加するコンポーネント。shared_ptr か生ポインタで。
		 * 
		 * @return 
		 * 追加されたコンポーネントの shared_ptr
		 */
	public:
		template<typename T>
		shared_ptr<T> add_component(shared_ptr<T> com) {
			if (com != nullptr) {
				com->object = this;
				components.push_back(com);
				componentIsAdded = true;
				return com;
			}
			return nullptr;
		}

		template<typename T>
		shared_ptr<T> add_component(T* com) {
			return add_component<T>(shared_ptr<T>(com));
		}

		/**
		 * @brief 
		 * コンポーネントを削除
		 * 
		 * @par 詳細
		 * 指定した型のコンポーネントを削除する。
		 * <br><br>
		 * 同じコンポーネントが複数あった場合、1つしか削除しない。
		 * 
		 * @note
		 * 削除したかどうかを bool で返すようにしたい。
		 * <br><br>
		 * 複数まとめて削除するような関数も用意したい。
		 */
		template<typename T>
		void delete_component() {
			for (int i = 0; i < (int)components.size(); ++i)
				if (std::dynamic_pointer_cast<T>(components[i])) {
					components.erase(components.begin() + i);
				}
				//if (typeid(T).name() == components[i]->get_type_name()) {
				//	components.erase(components.begin() + i);
				//	return;
				//}
		}





		// ******************** 子要素操作関数 ************************************************************************************** //

		/**
		 * @brief 
		 * 子オブジェクトを追加
		 * 
		 * @param obj
		 * shared_ptr か生ポインタを指定
		 * 
		 * @return
		 * 追加したオブジェクトのshared_ptr
		 * 
		 * @note
		 * add_objectみたいにコンストラクタからコール可能にできないかな
		 */
	public:
		template<typename T>
		shared_ptr<T> add_child(shared_ptr<T> obj, const int32 layerNo_ = 0);
		/*{
			if (obj != nullptr) {
				obj->gameState = gameState;
				children.push_back(obj);
				childrenIsAdded = true;
				gameState->set_child_to_camera(obj);
				return obj;
			}
			return nullptr;
		}*/

		//★ !!! コンストラクタからは呼ばない !!! ★//
		template<typename T>
		shared_ptr<T> add_child(T* obj, const int32 layerNo_ = 0);
		/*{
			return add_child(shared_ptr<T>(obj));
		}*/





		// ******************** オブジェクト操作関数 ******************************************************************************** //

		//void set_priority(const int priority_) {
		//	priority = priority_;
		//}

		/**
		 * @brief 
		 * オブジェクトを活性化する。
		 * 更新関数がコールされるようになる。
		 */
	public:
		void activate() {
			isActive = true;
		}

		/**
		 * @brief
		 * オブジェクトを非活性化する。
		 * 更新されなくなる。
		 */
		void deactivate() {
			isActive = false;
		}

		/**
		 * @brief 
		 * タグを追加する
		 * 
		 * @par 詳細
		 * 
		 * @param tag
		 * タグ
		 * 
		 * @note
		 * 重複解消の仕組みを足すべき。
		 */
		void add_tag(const string& tag) {
			tags.push_back(tag);
		}





		// ******************** Getter ********************************************************************************************** //

	public:
		/**
		 * @brief オブジェクトのIDを取得
		 */
		int get_id() const { return id; }

		/**
		 * @brief 所属するゲームステートのIDを取得
		 */
		int get_game_state_id() const;

		string get_type_name() const { return std::string(typeid(*this).name()); }

		/**
		 * @brief 所持するコンポーネントの数を取得
		 */
		int get_components_num() const { return (int)components.size(); }

		/**
		 * @brief 優先度を取得
		 */
		int get_priority() const { return priority; }

		/**
		 * @brief 活性状態を取得
		 */
		bool is_active() const { return isActive; }

		/**
		 * @brief オブジェクトが優先度ソートされているかどうか
		 */
		bool is_sorted() const { return isSorted; }

		/**
		 * @brief タグ一覧を取得
		 */
		const vector<string>& get_tags() const { return tags; }

		/**
		 * @brief 座標値を取得
		 */
		const Vec3& get_pos() const { return pos; }

		/**
		 * @brief 整数座標値を取得
		 */
		Vec3I get_posI() const { return pos.asPoint3D(); }

		/**
		 * @brief 座標値のXYをVec2で取得
		 */
		Vec2 get_pos_xy() const { return pos.xy(); }

		/**
		 * @brief 座標値のXYを整数座標値Vec2Iで取得
		 */
		Vec2I get_posI_xy() const { return { (int)pos.x, (int)pos.y}; }

		/**
		 * @brief 変更可能な座標値を取得
		 */
		Vec3& get_pos_ref() const { return pos; }

		/**
		 * @brief グローバル座標値を取得
		 *
		 * @par 詳細
		 * 子オブジェクトはローカル座標値を持つため、
		 * グローバル座標値がほしい場合はこの関数から取得する。
		 */
		const Vec3& get_global_pos() const {
			if (parent == nullptr)
				return pos;
			return parent->get_global_pos() + pos;
		}

		/**
		 * @brief グローバル整数座標値を取得
		 */
		Vec3I get_global_posI() const { return get_global_pos().asPoint3D(); }

		/**
		 * @brief グローバル座標値のXYをVec2で取得
		 */
		Vec2 get_global_pos_xy() const { return get_global_pos().xy(); }

		/**
		 * @brief グローバル座標値のXYを整数座標値Vec2Iで取得
		 */
		Vec2I get_global_posI_xy() const { return get_global_pos_xy().asPoint(); }

		/**
		 * @brief 座標値のZを取得
		 */
		double get_z() const { return pos.z; }

		/**
		 * @brief 子オブジェクトを持つかどうか
		 */
		bool has_children() const { return children.size() > 0; }

		/**
		* @brief 設定を取得
		*/
		const Config& get_config() const { return config; }

		/**
		* @brief 自身がカメラ内で所属するレイヤー番号を返す
		* 
		* @par 詳細
		* 番号が無効値で場合は例外を出す。
		* 呼び出す場所が悪い（コンストラクタなど）か、
		* SGM2の実装にミスがあるかのどちらかだからか。
		* コンストラクタからは呼ばないこと。
		*/
		int32 get_layer_no() const {
			if (layerNo) {
				return layerNo.value();
			}
			throw std::runtime_error("レイヤー番号が無効値。get_layer_noをコンストラクタから呼び出している可能性があります。そうでなければSGM2のバグです。レイヤー番号の初期化処理周辺を修正してください。");
		}

		string get_type() const {
			return string(typeid(*this).name());
		}

		/**
		 * @brief 
		 * コンポーネントを取得。
		 * 
		 * @par 詳細
		 * 型名を指定してコンポーネントのweak_ptrを取得する。
		 * <br><br>
		 * ダイナミックキャストが入るため若干遅い。
		 * ゆえにreadyやコンストラクタからコールすべきだが、
		 * だったらadd_componentから取得してしまう方がいいだろう。
		 * どうしても後から取得しなければならないケースでのみ使う。
		 * 
		 * @return 
		 * コンポーネント
		 * 
		 * @note
		 * 戻り値をweak_ptrにしたのはいい判断だと思う。
		 * それならadd_componentの戻り値だってweak_ptrでいいよね。
		 * あっちはshared_ptrだったはず。
		 */
		template<typename T>
		weak_ptr<T> get_component() {
			for (int i = 0; i < (int)components.size(); ++i)
				if (const auto& ptr = std::dynamic_pointer_cast<T>(components[i]))
					return ptr;
				//if (typeid(T).name() == components[i]->get_type_name())
				//	return std::static_pointer_cast<T>(components[i]);
			return weak_ptr<T>();//nullptr;
		}

		/**
		 * @brief 
		 * constなshared_ptrとしてコンポーネントを取得
		 * 
		 * @par 詳細
		 * なぜこの関数を用意したのか覚えていない。
		 * 
		 * @warning
		 * 多分これ使っちゃうとまずい。
		 * <br><br>
		 * 所有権を持つポインタが増えるので、オブジェクトを破棄した際にコンポーネントだけ残る可能性がある。
		 * これは子オブジェクトにも言えるだろう。
		 * ちゃんと実装見てないから何とも言えないけど。
		 * 
		 * @return 
		 * コンポーネント
		 */
		template<typename T>
		shared_ptr<const T> get_component_cshare() const {
			for (int i = 0; i < (int)components.size(); ++i)
				if (const auto& ptr = std::dynamic_pointer_cast<T>(components[i]))
					return ptr;
				//if (typeid(T).name() == components[i]->get_type_name())
				//	return std::static_pointer_cast<T>(components[i]);
			return shared_ptr<T>();//nullptr;
		}

		// !!!★ まだテストしてません ★!!!
		/**
		 * @brief
		 * 型指定で子オブジェクトを取得
		 * 
		 * @par 詳細
		 * 孫まで探索し、該当するオブジェクトを1つだけ返す。
		 * 
		 * @return 
		 * オブジェクト
		 * 
		 * @note
		 * 複数取得する関数もほしい。
		 */
		template<typename T>
		weak_ptr<T> get_child_by_type() {
			for (auto& object : children) {
				if (const auto& ptr = std::dynamic_pointer_cast<T>(object))
					return ptr;
				//if (typeid(T).name() == object->get_type())
				//	return std::static_pointer_cast<T>(object);
				auto ret = object->get_child_by_type<T>();
				if (!ret.expired())
					return ret;
			}
			return weak_ptr<T>();
		}

		// 型指定で子オブジェクトを取得
		// 孫の探索は行わない
		/**
		 * @brief
		 * 型指定でオブジェクトを取得
		 * 
		 * @par 詳細
		 * 孫の探索は行わない。
		 * 子の中から該当するオブジェクトを1つだけ返す。
		 * 
		 * @return 
		 * オブジェクト
		 * 
		 * @note
		 * 複数取得する関数もほしい。
		 */
		template<typename T>
		weak_ptr<T> get_child_from_top_by_type() {
			for (auto& object : children) {
				if (const auto& ptr = std::dynamic_pointer_cast<T>(object))
					return ptr;
				//if (typeid(T).name() == object->get_type())
				//	return std::static_pointer_cast<T>(object);
			}
			return weak_ptr<T>();
		}





		// ******************** ユーザー定義関数 ************************************************************************************ //

		// gameStateを介して何かをするならコンストラクタではだめ
		// ready()は初回update時にコールされるため、GSの管理下に入ってから呼ばれる
		/**
		 * @brief
		 * 準備関数
		 * 
		 * @par 詳細
		 * 初回更新時にコールされる。
		 * <br><br>
		 * コンストラクタと違って、この関数がコールされる時点ではゲームステートの管理下に入っている。
		 * つまり、自身が所属するGS等にアクセスする場合はコンストラクタが使えない。
		 * この関数を利用すること。
		 * <br><br>
		 * 実行タイミングについて、このページ上部や「コンポーネント」のページを参照。
		 */
	private:
		virtual void ready() {}

		/**
		 * @brief
		 * 更新関数
		 * 
		 * @par 詳細
		 * 毎フレームGSや親オブジェクトからコールされる。
		 * <br><br>
		 * falseを返したオブジェクトは破棄される。
		 * 
		 * @return 
		 * 削除フラグ
		 */
		virtual bool update() { return true; }

		// 引数にはスクリーン座標値と拡大率が渡される
		// カメラで計算される値なので、調整したい場合はユーザー定義のカメラを作成する
		// 使い方によっては引数を無視してもよかったり
		/**
		 * @brief
		 * 見た目を定義
		 * 
		 * @par 詳細
		 * オブジェクトの見た目を定義する。
		 * <br><br>
		 * 描画位置やサイズ等、描画に必要な情報は引数から得られたものを使うこと。
		 * これらは「カメラ」が計算して渡すものであり、利用しないとカメラが十分に機能しない。
		 * <br><br>
		 * ここでの描画位置はスクリーン座標値となる。
		 * この辺は「Camera」や「Screen」のページに詳しく書いているかも。
		 * <br><br>
		 * 自身が描画されるスクリーンの数だけ毎フレームコールされる。
		 * 
		 * @param dp
		 * 描画用データ
		 */
		virtual void draw(const DrawingProperties& dp) const {}


		void init_layer_no(const int32 layerNo_) { layerNo = layerNo_; }





		// ******************** Setter ********************************************************************************************** //

		//void set_game_state_id(const int id) {
		//	gameSstateId = id;
		//}





		// ******************** 更新関数と、その内部で使うやつ ********************************************************************** //

	private:
		void update_components() {

			// コンポーネントが追加されていれば優先度に従ってソートする
			// コンポーネントのupdateからコンポーネントを追加した場合、次のフレームまではソートされない
			// 従って、追加分の更新処理は次フレームからの開始とする
			// priorityに反する順序での処理を防ぐためである
			if (componentIsAdded) {
				std::sort(components.begin(), components.end(), [](const auto& v0, const auto& v1) {
					return v0->get_priority() > v1->get_priority();
				});
				for (auto& component : components)
					component->isSorted = true;
				componentIsAdded = false;
			}

			// コンポーネント内でコンポーネントが付加された場合、範囲forではうまくいかない
			//		for (auto& com : components)
			// sizeを先に取得すれば、処理中に追加されたコンポーネントは処理せずに済む
			// ただし、途中で削除が発生した場合に範囲外アクセスする危険がある
			//		int length = (int)components.size();
			// 現在はフラグ操作で対処している
			for (int i = 0; i < (int)components.size(); ++i) {
				auto& component = components.at(i);
				if (component->is_active() && component->is_sorted())
					component->trigger_update();
			}
		}

		void sort_children_by_priority() {
			// 後ろから更新するので昇順に
			std::sort(children.begin(), children.end(), [](const auto& v0, const auto& v1) {
				return v0->get_priority() < v1->get_priority();
			});
		}

		bool trigger_update() {

			// 子オブジェクトのソート
			if (childrenIsAdded) {
				sort_children_by_priority();
				//sort_children_by_z();

				for (auto& obj : children)
					obj->isSorted = true;
				childrenIsAdded = false;
			}

			// 子オブジェクトの更新と削除
			vector<int> deleteIdList;
			for (int i = (int)children.size() - 1; i >= 0; --i) {
				auto& obj = children.at(i);
				if (obj->is_active() && obj->is_sorted())
					if (!obj->trigger_update()) {
						deleteIdList.push_back(obj->get_id());
						children.erase(children.begin() + i);
						//objectsDraw->erase(objectsDraw->begin() + i);
					}
			}

			update_components();

			if (!isInitialized) {
				ready();
				isInitialized = true;
			}

			return update();
		}

		void trigger_draw(const DrawingProperties& dp) const {
			draw(dp);
			for (const auto com : components) {
				com->draw(dp);
			}
		}
	};
}
