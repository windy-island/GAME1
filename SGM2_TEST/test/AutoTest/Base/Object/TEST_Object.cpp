
#include "catch2.hpp"
#include "sgmPlus.h"
#include "./TestData.hpp"

namespace SGM2_TEST::Object {

	TEST_CASE("オブジェクト", "[base]") {
		gsm.reset();
		num = 0;



		/********************************************************************************************************************************************************************/

		SECTION("主要機能のチェック") {

			/*
			* ● オブジェクトのaddができるか。readyとupdateが正常に動作するか。
			* 1: 0 + 3 = 3			[ GS_8: 1, Obj8: 0 ]
			* 2: 3 + 1 + 2 + 3 = 9	[ GS_8: 2, Obj8: 1 ]
			* 3: 9 + 2 + 3 = 14		[ GS_8: 3, Obj8: 2 ]
			*/
			SECTION("基本機能（add, ready, update）の動作チェック") {
				gsm.reset(GameStateManager::create_instance(new GS_A()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 14);
			}

			/*
			* ● コンストラクタでPositionやpriorityの値がセットできているか
			* 1: 0 + 3 = 3						[ GS_9: 1, Obj9: 0 ]
			* 2: 3 + 10 + 1 + 2 + 3 + 3 = 22	[ GS_9: 2, Obj9: 1 ]
			* 3: 22 + 1 + 2 + 3 + 3 = 31		[ GS_9: 3, Obj9: 2 ]
			*/
			SECTION("コンストラクタで priroty や position が正しくセットされているか") {
				gsm.reset(GameStateManager::create_instance(new GS_B()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 31);
			}
		}

		// add_componet（コンポーネントの方でチェック済み）
		// delete_component
		// add_child
		// child_main_functionality
		// priority（子込みでチェック）
		// active
		// tag
		// get_component_num
		// get_compoent
		// get_child
		// get_child_from_top
	}
}
