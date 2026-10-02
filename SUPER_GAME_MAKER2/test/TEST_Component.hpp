#pragma once

#include "sgmPlus.h"
#include "TestMacro.h"

namespace SGM2_TEST {

	class TEST_Component {

	public:

		bool test() {
			TEST_ROOT_HEADER("Component");

			TEST_main_functionality();
			TEST_priority();
			TEST_waiting_activate();
			TEST_activate();

			TEST_ROOT_FOOTER;
		}

	private:

		// 主要機能のチェック
		bool TEST_main_functionality() {
			TEST_FUNC_HEADER;

			// ● コンポーネントのadd（生ポインタ）ができるか。readyとupdateが正常に動作するか。
			// 更新の順番は、[子Obj] -> [Com の ready & update] -> [Obj の ready & update] -> [GS の ready & update]である。
			// よって、処理の順序は以下
			// 
			// ・初回更新
			// GS.ready( Obj追加 ) -> GS.update( num += 4 )
			// これで num = 4
			// 
			// ・更新2回目
			// Obj.ready( Com追加 ) -> Obj.update( num += 3 ) -> GS.update( num += 4)
			// これで num = 11
			// 
			// ・更新3回目
			// Com.ready( num += 1 ) -> Com.update( num += 2 ) -> Obj.update( num += 3 ) -> GS.update( num += 4)
			// num = 21
			// 
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_4()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 21), "基本機能（add, ready, update）に不具合あり", "予期せぬ例外");

			// ● 上記のコンストラクタ別バージョン版
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_4_1>()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 21), "基本機能（add, ready, update）に不具合あり", "予期せぬ例外");

			TEST_FUNC_FOOTER("主要な機能のチェック（objectへのadd、readyやupdateの動作確認など）");
		}

		// 優先度の通りに更新されるか
		bool TEST_priority() {
			TEST_FUNC_HEADER;

			/*
			* ● priorityの動作チェック
			* 1: 0 + 4 = 4
			* 2: 4 + 3 + 4 = 11
			* 3: (11 + 2 + 10) * 2 + 3 + 4 = 53
			*/
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_5>()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 53), "優先度が機能していない", "予期せぬ例外");

			TEST_FUNC_FOOTER("priority");
		}

		// コンポーネントから追加（更新中に追加）された場合、
		// ソートできないので次フレームから更新される。
		// その辺ちゃんと動いているか。
		bool TEST_waiting_activate() {
			TEST_FUNC_HEADER;

			/*
			* ● コンポーネントやオブジェクトからコンポーネント追加をやって動作確認
			* 更新回数: numの値 [それぞれの更新回数]
			* 1: 0								[ GS_6: 1, Obj6: 0 ]
			* 2: 0 + 3 = 3						[ GS_6: 2, Obj6: 1 ]
			* 3: 3 + 3 = 6						[ GS_6: 3, Obj6: 2, Com6_0: 0 ]
			* 4: 6 + 1 + 2 + 3 = 12				[ GS_6: 4, Obj6: 3, Com6_0: 1 ]
			* 5: 12 + 2 + 3 = 17				[ GS_6: 5, Obj6: 4, Com6_0: 2, Com6_1: 0 ] (コンポーネントから追加した場合も更新は次の回から)
			* 6: 17 + 3 + 7 + 2 + 3 = 32		[ GS_6: 6, Obj6: 5, Com6_0: 3, Com6_1: 1 ]
			*/
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_6>()));
				for (int i = 0; i < 6; ++i) {
					gsm->update();
				}
			}, (num == 32), "コンポーネント追加時の挙動に不具合", "予期せぬ例外");

			TEST_FUNC_FOOTER("wait activate");
		}

		// 活性状態の変更がちゃんと反映されるか
		bool TEST_activate() {
			TEST_FUNC_HEADER;

			/*
			* 1: 0					[ GS_7: 1, Obj7: 0 ]
			* 2: 0 + 1 = 1			[ GS_7: 2, Obj7: 1 , Com7: 0] 以下、count = Obj7の更新回数となる
			* 3: 1 + 2 + 3 + 1 = 7	[ GS_7: 3, Obj7: 2 , Com7: 1] Com7がdeactivateされるが、それはCom7の更新後
			* 4: 7 + 1 = 8			[ GS_7: 4, Obj7: 3 , Com7: 1] Com7は停止中だが、ここでactivateされる
			* 5: 8 + 3 + 1 = 12		[ GS_7: 5, Obj7: 4 , Com7: 2] Com7の更新後にdeactivateされる
			* 6: 12 + 1 = 13		[ GS_7: 6, Obj7: 5 , Com7: 2]
			*/
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_7>()));
			for (int i = 0; i < 6; ++i) {
				gsm->update();
			}
				}, (num == 13), "活性状態の操作が効いていない", "予期せぬ例外");

			TEST_FUNC_FOOTER("activate");
		}

		//bool TEST_get_priority() {}
		//bool TEST_is_activate() {}
		//bool TEST_is_sorted() {}
		//bool TEST_get_type_name() {}
	};
}
