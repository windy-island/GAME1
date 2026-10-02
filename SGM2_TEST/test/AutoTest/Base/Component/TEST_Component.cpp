
#include "catch2.hpp"
#include "sgmPlus.h"
//#include "SAMPLE_GameState.h"
#include "./TestData.hpp"

namespace SGM2_TEST::Component {

	TEST_CASE("コンポーネント", "[base]") {
		gsm.reset();
		num = 0;



		/********************************************************************************************************************************************************************/

		SECTION("主要機能のチェック") {

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
			SECTION("add(生ポインタ), ready, updateの動作チェック") {
				gsm.reset(GameStateManager::create_instance(new GS_A()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 21);
			}

			// 上記のコンストラクタ別バージョン版
			SECTION("add(shared_ptr), ready, updateの動作チェック") {
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_A1>()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 21);
			}
		}



		/********************************************************************************************************************************************************************/

		SECTION("優先度順に更新されるか") {

			/*
			* ● priorityの動作チェック
			* 1: 0 + 4 = 4
			* 2: 4 + 3 + 4 = 11
			* 3: (11 + 2 + 10) * 2 + 3 + 4 = 53
			*/
			SECTION("Priorityの動作チェック") {
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_B>()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 53);
			}
		}



		/********************************************************************************************************************************************************************/

		SECTION("コンポーネントから更新中に追加された場合、次フレームまで起動を更新を待機してるか") {

			/*
			* ● コンポーネントやオブジェクトからコンポーネント追加をやって動作確認
			* 更新回数: numの値 [それぞれの更新回数]
			* 1: 0								[ GS_C: 1, ObjC: 0 ]
			* 2: 0 + 3 = 3						[ GS_C: 2, ObjC: 1 ]
			* 3: 3 + 3 = 6						[ GS_C: 3, ObjC: 2, ComC1: 0 ]
			* 4: 6 + 1 + 2 + 3 = 12				[ GS_C: 4, ObjC: 3, ComC1: 1 ]
			* 5: 12 + 2 + 3 = 17				[ GS_C: 5, ObjC: 4, ComC1: 2, ComC0: 0 ] (コンポーネントから追加した場合も更新は次の回から)
			* 6: 17 + 3 + 7 + 2 + 3 = 32		[ GS_C: 6, ObjC: 5, ComC1: 3, ComC0: 1 ]
			*/
			SECTION("コンポーネント、オブジェクトから追加した場合の動作確認。") {
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_C>()));
				for (int i = 0; i < 6; ++i) {
					gsm->update();
				}
				CHECK(num == 32);
			}
		}



		/********************************************************************************************************************************************************************/

		SECTION("活性状態の変更が反映されているか") {
			
			/*
			* 1: 0					[ GS_D: 1, ObjD: 0 ]
			* 2: 0 + 1 = 1			[ GS_D: 2, ObjD: 1 , ComD: 0] 以下、count = Obj7の更新回数となる
			* 3: 1 + 2 + 3 + 1 = 7	[ GS_D: 3, ObjD: 2 , ComD: 1] ComDがdeactivateされるが、それはComDの更新後
			* 4: 7 + 1 = 8			[ GS_D: 4, ObjD: 3 , ComD: 1] ComDは停止中だが、ここでactivateされる
			* 5: 8 + 3 + 1 = 12		[ GS_D: 5, ObjD: 4 , ComD: 2] ComDの更新後にdeactivateされる
			* 6: 12 + 1 = 13		[ GS_D: 6, ObjD: 5 , ComD: 2]
			*/
			SECTION("動作確認") {
				gsm.reset(GameStateManager::create_instance(std::make_unique<GS_D>()));
				for (int i = 0; i < 6; ++i) {
					gsm->update();
				}
				CHECK(num == 13);
			}
		}

		//bool TEST_get_priority() {}
		//bool TEST_is_activate() {}
		//bool TEST_is_sorted() {}
		//bool TEST_get_type_name() {}
	}
}
