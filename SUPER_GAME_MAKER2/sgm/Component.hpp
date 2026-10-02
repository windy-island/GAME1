/*****************************************************************//**
 * @file   Component.h
 * @brief  派生して"コンポーネント"を作成できます
 * 
 * @date   January 2024
 *********************************************************************/
#pragma once

#include "std.h"
#include "DrawingProperties.hpp"

namespace SGM2 {

	class Object;

	/**
	 * @brief Com: コンポーネントを作成する際の基底クラス
	 * 
	 * @details
	 * このクラスを継承してコンポーネントを作成する。
	 * <br><br>
	 * 優先度を指定することで、コンポーネントの更新順を制御することが可能。
	 * 優先度を指定しない場合は一律で0となり、値の大きいコンポーネントから順番に更新処理が実行される。
	 * <br><br>
	 * 活性化、非活性化状態を切り替えることで更新の停止もできる。
	 * 
	 * @important
	 * **★ 優先度の重複**
	 * <br>
	 * 優先度が同じ場合の更新順は未定義。
	 * 追加順に更新されるわけではない。
	 * 確か逆順だったと思うが、変更される可能性があるため当てにすべきではない。
	 * 順番が大事ならば必ず優先度を指定すること。
	 * <br><br>
	 * **★ ready や update の実行順序**
	 * <br>
	 * 実行順番は下記のようになる。<br>
	 * 1 コンポーネント [ ready  ]<br>
	 * 2 コンポーネント [ update ]<br>
	 * 3 オブジェクト [ ready  ]<br>
	 * 4 オブジェクト [ update ]<br>
	 * 5 ゲームステート [ ready  ]<br>
	 * 6 ゲームステート [ update ]
	 * <br><br>
	 * **★ コンポーネント追加時の始動タイミング**
	 * <br>
	 * オブジェクトのupdateなど、
	 * コンポーネントのreadyやupdateが終わった後の処理で追加した場合、
	 * 始動は次のフレームからである。
	 * <br><br>
	 * コンポーネントのupdateから追加した場合、
	 * つまり更新中に追加した場合も次のフレームから始動する。
	 * 更新済みのコンポーネントがいるせいで、優先度ソートが実行できないからだ。
	 * これにより優先度に反する順序で更新されることを防ぐ。
	 */
	class Component {

		friend class Object;

	public:

		/**
		 * @brief 
		 * コンストラクタ
		 * 
		 * @par 詳細
		 * 優先度を設定したい場合は引数に指定。
		 * 必要がなければ引数なしでOK。
		 * 
		 * @param priority_
		 * 優先度。これが高いコンポーネントから順に更新処理が走る。
		 */
		constexpr Component(const int priority_ = 0) :
			object(nullptr),
			isActive(true),
			isSorted(false),
			priority(priority_),
			isFirstUpdate(true) {
		}

		virtual ~Component() = default;

		/**
		 * @brief 
		 * コンポーネントを活性化する。
		 * 更新処理がコールされる状態であり、デフォルトでこの状態になっている。
		 */
		void activate() {
			isActive = true;
		}

		/**
		 * @brief 
		 * コンポーネントを非活性状態にする。
		 * 更新処理が呼ばれなくなる。
		 */
		void deactivate() {
			isActive = false;
		}

		/**
		 * @brief 
		 * コンポーネントの優先度を取得。
		 */
		int get_priority() const { return priority; }

		/**
		 * @brief 
		 * コンポーネントの活性状態を取得。
		 */
		bool is_active() const { return isActive; }

		/**
		 * @brief 
		 * コンポーネントがオブジェクト内で優先度ソートされているかを返す。
		 * 内部処理で扱うのみであり、ユーザーが知る必要はない。
		 */
		bool is_sorted() const { return isSorted; }

	private:
		/**
		 * @brief 
		 * 準備関数
		 * 
		 * @par 詳細
		 * 初回更新時にコールされる。
		 * <br><br>
		 * コンストラクタと違って、この関数がコールされる時点ではオブジェクトの管理下に入っている。
		 * get_componet等はこの関数で済ますべきだろう。
		 * <br><br>
		 * 実行タイミング等の詳しい話は、上の方に書いてある。
		 */
		virtual void ready() {}

		/**
		 * @brief 更新関数
		 * 
		 * @par 詳細
		 * 毎フレーム親オブジェクトからコールされる。
		 * <br><br>
		 * 詳しい話は上の方に書いておいた。
		 * 
		 * @param obj
		 * コール元の親オブジェクト
		 */
		virtual void update() {}

		/**
		 * @brief  描画関数
		 * 
		 * @par 詳細
		 * 渡されるDPはオブジェクトに渡されたものがそのまま流用されている。
		 * 詳しくは「Object」のページに載ってるかも。
		 * <br><br>
		 * オブジェクトのdrawがコールされた後でコンポーネントのdrawがコールされる。
		 * 
		 * @note
		 * 複数のコンポーネントがあった場合にdraw呼び出し順序を制御する機構はない。
		 * 一応作ってみてもいいだろう。
		 * 
		 * @param dp 描画用データ
		 */
		virtual void draw(const DrawingProperties& dp) const {}

		/**
		 * @brief 自身のクラス名を取得
		 * 
		 * @par 詳細
		 * 実行時型情報を用いてクラス名を返却する。
		 * 派生した場合は派生クラス名を返すため、コンポーネントの判別に利用可能。
		 * Object の get_componet で利用している？
		 *
		 * @warning
		 * もう使っていなかったと思う。
		 * 複数回継承していた場合、この名前比較では正しく判別できない。
		 * dynamic cast ができるかどうかで判定すべき。
		 * 
		 * @return クラス名
		 */
		string get_type_name() const { return std::string(typeid(*this).name()); }

		/**
		 * @brief 
		 * 更新関数等のラッパー
		 * 
		 * @par 詳細
		 * readyやupdateをコールする。
		 * オブジェクトから毎ループコールされるのはこの関数。
		 */
		void trigger_update() {
			if (isFirstUpdate) {
				ready();
				isFirstUpdate = false;
			}
			update();
		}

		
		//void set_priority(const int priority_) {
		//	priority = priority_;
		//}

	protected:
		Object* object;
	private:
		bool isActive;
		bool isSorted;
		const int priority;
		bool isFirstUpdate;
	};
}

