#pragma once

#include "std.h"
//#include "GameState.h"
//#include "Object.h"

namespace SGM2 {

	class GameState;
	class Object;

	/**
	 * @brief 
	 * GSM: ゲームステートを管理
	 * 
	 * @details
	 * <br>
	 * シングルトン。
	 * 
	 * コンストラクタがprivateなため、インスタンスの生成には create_instance 関数を用いること。
	 * <br><br>
	 * GSMはsgmPlus.hにてグローバル領域に確保されている。
	 * <br><br>
	 * あらゆるゲームステートやオブジェクト、コンポーネントは、このGSMにアクセスすることでシーン遷移を予約することができる。
	 */
	class GameStateManager {

		friend class TEST_GameStateManager;

	public:
		/**
		 * @brief 
		 * GSMのインスタンスを生成
		 * 
		 * 
		 * @param initialState
		 * 初期状態となるゲームステートのポインタを指定。
		 * nullptrは不可。
		 * 
		 * @return 
		 * 生成したGSMインスタンスのポインタ。
		 * 生成できなかった場合は例外を投げる。
		 * 
		 * @par 詳細
		 * コンストラクタは使えないため、この関数で生成する。
		 * <br>
		 * GSMはシングルトンであるため、同時に生成できるインスタンスは1つまで。
		 * 削除すれば新たなインスタンスを生成することが可能。
		 */
		NODISCARD static GameStateManager* create_instance(unique_ptr<GameState> initialState);
		NODISCARD static GameStateManager* create_instance(GameState* initialState);

		GameStateManager(GameStateManager&&) noexcept;
		GameStateManager& operator=(GameStateManager&&) noexcept;
		~GameStateManager();

		/**
		 * @brief 
		 * GSの状態遷移を予約する
		 * 
		 * @param caller 呼び出し元のゲームステートID
		 * @param destGameState 遷移先のGS。nullptrで遷移先なしの指定も可。
		 * @param isPop 呼び出し元のGSをポップするか否か
		 * @param deactivateFlag 遷移と同時に現在のGSを非活性状態に変更するか否か
		 * 
		 * @par 詳細
		 * 先述したように、GSMのインスタンスはグローバル領域に置く前提である。
		 * したがって、この関数はあらゆる場所からコールが可能となっている。
		 * <br><br>
		 * この関数が行うのはあくまで予約。
		 * 引数をそのまま「TransitionData」に格納し、それを遷移予定キューに追加するのみである。
		 * <br><br>
		 * 第1引数には呼び出し元のゲームステートIDを指定するが、
		 * ここは呼び出し元のオブジェクトやGSをそのまま入れても構わない。
		 * オーバーロード版がある。
		 * <br><br>
		 * 遷移処理を行うのはtransition関数で、
		 * こちらはGSに含まれる全ての更新処理が終わった後に自動でコールされる。
		 * 詳しくはそちらを参照。
		 */
		void reserve_transition(const uint32 gameStateId, GameState* destGameState, const bool isPop = true, const bool deactivateFlag = true);
		void reserve_transition(const Object& caller, GameState* destGameState, const bool isPop = true, const bool deactivateFlag = true);
		void reserve_transition(const GameState& caller, GameState* destGameState, const bool isPop = true, const bool deactivateFlag = true);

		/**
		 * @brief 
		 * 更新処理。ゲームのメインループ内で毎フレームコールすること。
		 * 
		 * @par 詳細
		 * アクティブなGSの更新処理を呼び出す。
		 * <br><br>
		 * 下層のGSから順に更新され、トップステートが最後に更新される。
		 * 更新順序の制御はできないが、各GSは独立すべきであるため問題ない。
		 * そもそも、複数のGSがアクティブになるケースはあまりなさそう。
		 * GS内に属するオブジェクトやコンポーネントには順序制御がある。
		 * <br><br>
		 * <span class="marker">全て更新した後で、シーン遷移関数をコールする。</span>
		 * 遷移の予約があった場合、このタイミングでシーン遷移が実行される。
		 * 
		 * @return
		 * GSスタックが空ならfalse。それ以外はtrue。
		 */
		bool update();	// 終了状態（スタックが空）でfalseを返す

	private:
		/// @cond
		struct TransitionData {
			int callersGameStateId = 0;
			GameState* dest = nullptr;
			bool isPop = true;
			bool deactivateFlag = true;
		};
		/// @endcond

		GameStateManager(unique_ptr<GameState> initialState);
		GameStateManager(const GameStateManager&) = delete;
		GameStateManager& operator=(const GameStateManager&) = delete;

		/**
		 * @brief 
		 * 指定されたGSがスタックのトップにいるのかを判定
		 * 
		 * @param gameStateId 調べるGSのID
		 * 
		 * @return トップならtrue
		 */
		bool is_top_state(const int callersGameStateId) const;

		/**
		 * @brief 
		 * シーン遷移
		 * 
		 * @par 詳細
		 * 遷移予定キューが空になるまで予約された遷移を実行。
		 * 複数の予約があった場合は、予約順に遷移が実行される。
		 * <br><br>
		 * 遷移依頼元がトップステートでなかった場合、上位のGSは全てポップする。
		 * これにより強制的にトップステート化させ、その上で遷移処理を実行。
		 * <br><br>
		 * 遷移先の指定がなかった場合は、トップステートをアクティブ化する。
		 * 遷移先がある場合は、そのGSをアクティブ化する。
		 * <br><br>
		 * deactivateFlagは遷移元の活性状態を指定するのみであり、
		 * 遷移によってアクティブなGSがなくなることはない。
		 */
		void transition();

		vector<unique_ptr<GameState>> gameStateStack;
		queue<TransitionData*> transitionQueue;
		int nextGameStateId;

		static int instanceNum;

	public:
		// --- DEBUG用関数---//　
		vector<unique_ptr<GameState>>& DEBUG_get_game_state_stack();
	};
}
