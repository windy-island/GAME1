#pragma once

#include "sgmPlus.h"
#include "TestMacro.h"

namespace SGM2_TEST {

	class TEST_Object {

	public:

		bool test() {
			TEST_ROOT_HEADER("Object");

			TEST_main_functionality();
			// add_component（コンポーネントの方でチェック済み）
			TEST_delete_component();
			// add_child
			// child_main_functionality
			// priority（子込みでチェック）
			// active
			// tag
			// get_component_num
			// get_component
			// get_child
			// get_child_from_top

			TEST_ROOT_FOOTER;
		}

	private:

		// 主要機能のチェック
		bool TEST_main_functionality() {
			TEST_FUNC_HEADER;

			/*
			* ● オブジェクトのaddができるか。readyとupdateが正常に動作するか。
			* 1: 0 + 3 = 3			[ GS_8: 1, Obj8: 0 ]
			* 2: 3 + 1 + 2 + 3 = 9	[ GS_8: 2, Obj8: 1 ]
			* 3: 9 + 2 + 3 = 14		[ GS_8: 3, Obj8: 2 ]
			*/ 
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_8()));
				for (int i = 0; i < 3; ++i)
					gsm->update();
			}, (num == 14), "基本機能（add, ready, update）に不具合あり", "予期せぬ例外");

			/*
			* ● コンストラクタでPositionやpriorityの値がセットできているか
			* 1: 0 + 3 = 3						[ GS_9: 1, Obj9: 0 ]
			* 2: 3 + 10 + 1 + 2 + 3 + 3 = 22	[ GS_9: 2, Obj9: 1 ]
			* 3: 22 + 1 + 2 + 3 + 3 = 31		[ GS_9: 3, Obj9: 2 ]
			*/
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_9()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
			}, (num == 31), "コンストラクタの引数が反映されていない？", "予期せぬ例外");

			TEST_FUNC_FOOTER("主要機能のチェック（GSへのadd、readyやupdateの動作確認など）");
		}

		bool TEST_delete_component() {
			TEST_FUNC_HEADER;

			// ● 

			TEST_FUNC_FOOTER("delete_component");
		}
	};
}
