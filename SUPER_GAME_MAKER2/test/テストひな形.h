#pragma once

#include "sgmPlus.h"
#include "TestMacro.h"

namespace SGM2_TEST {

	class TEST_Object {

	public:
		bool test() {
			TEST_ROOT_HEADER("Object");

			TEST_main_functionality();

			TEST_ROOT_FOOTER;
		}

	private:
		// 主要機能のチェック
		bool TEST_main_functionality() {
			TEST_FUNC_HEADER;

			// ● オブジェクトのaddができるか。readyとupdateが正常に動作するか。
			SHOULD_TRUE({
				gsm.reset(GameStateManager::create_instance(new GS_8()));
			for (int i = 0; i < 3; ++i)
				gsm->update();
				}, (num == 16), "基本機能（add, ready, update）に不具合あり", "予期せぬ例外");

			TEST_FUNC_FOOTER("主要機能のチェック（GSへのadd、readyやupdateの動作確認など）");
		}
	};
}
