#pragma once

#include "sgmPlus.h"
#include "TestMacro.h"

namespace SGM2_TEST {

	class TEST_GameStateManager {

	public:

		bool test() {
			TEST_ROOT_HEADER("GameStateManager");

			TEST_create_instance();
			TEST_reserve_transition();
			//std::cout << "< update >: ";
			//std::cout << "< is_top_state >: ";

			TEST_ROOT_FOOTER;
		}

	private:

		// インスタンスを生成できるか
		bool TEST_create_instance() {
			TEST_FUNC_HEADER;

			// ここは生ポインタじゃなくてuniqueを使う
			// 例外発生時にdeleteできなくなるから


			// ◆マクロなしだとこうなる（カウントなし）
			//try {
			//	unique_ptr<GameStateManager> pGSM(GameStateManager::create_instance(nullptr));
			//	errMessages.push_back("初期ステートnullなのに生成できた");
			//}
			//catch (MAYBE_UNUSED const std::exception& e) {}

			// ● 初期ステートなし生成でちゃんと例外を出すか
			SHOULD_THROW_EXCEPTION({
				unique_ptr<GameStateManager> pGSM(GameStateManager::create_instance(nullptr));
			}, "初期ステートnullなのに生成できた");
		

			// ● 正しくGSMが生成できるか
			unique_ptr<GameStateManager> pGSM1, pGSM2;

			SHOULD_NOT_THROW_EXCEPTION({
				pGSM1.reset(GameStateManager::create_instance(new GS_A()));
			}, "インスタンスが生成できません");

			// ● 2つ目の生成で例外を出すか（シングルトンなのでできたら困る）
			SHOULD_THROW_EXCEPTION({
				pGSM2.reset(GameStateManager::create_instance(new GS_A()));
			}, "複数のインスタンスが生成できてしまいます");
			

			TEST_FUNC_FOOTER("create_instance");
		}

		// 予約関数ではあるが、遷移処理までテストしてしまう
		bool TEST_reserve_transition() {
			TEST_FUNC_HEADER;

			// ◆マクロなしだとこうなる（カウントなし）
			//try {
			//	gsm.reset(GameStateManager::create_instance(new GS_1()));
			//	for (int i = 0; i < 2; ++i)
			//		gsm->update();
			//	if (num != 23)
			//		errMessages.push_back("遷移処理、更新処理に不具合あり");
			//}
			//catch (const std::exception& e) {
			//	errMessages.push_back("予期せぬ例外");
			//}
			//gsm.reset();

			// ● シーンから遷移させられるか
			// 初回更新時は ready と update が走る
			// 最初は GS1 でこれらが実行され、num = 5 となる
			// 初回更新終了後に予約されていた遷移が実行され、2回目の更新では GS2 の ready と update が走る
			// これで num = 23
			// 次の更新では遷移なしなので GS2 の update が実行され、
			// num == 31 となる
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_1()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 31), "シーンからの遷移処理、更新処理に不具合あり", "予期せぬ例外");

			/*
			* ● オブジェクトから遷移させられるか
			* 1: 0 + 3 = 3			[ GS_3: 1, Obj1: 0 ]
			* 2: 3 + 4 + 3 = 10		[ GS_3: 2, Obj1: 1 ] 遷移予約が入る
			* 3: 10 + 10 + 8 = 28	[ GS_2: 1 ]
			*/
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_3()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 28), "オブジェクトからの遷移処理、更新処理に不具合あり", "予期せぬ例外");

			// ● コンポーネントから遷移させられるか
			// ● オーバーライド版は動いているか
			// ● プッシュオンリー
			// ● ポップオンリー
			// ● ポップしてからプッシュ
			// ● トップ以外からの遷移要求
			TEST_FUNC_FOOTER("reserve_transition, transition");
		}

		bool TEST_update() {

		}

		bool TEST_is_top_state() {

		}

		bool TEST_transition() {

		}
	};
}
