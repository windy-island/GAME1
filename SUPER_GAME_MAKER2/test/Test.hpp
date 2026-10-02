#pragma once

#include "TEST_GameStateManager.hpp"
#include "TEST_Component.hpp"
#include "TEST_Object.h"
#include "TEST_Camera.hpp"
#include "Siv3D.hpp"

namespace SGM2_TEST {

	void ready_console();

	bool test() {
		ready_console();

		std::cout << "\n((( TEST START )))\n\n";

		// テスト
		{
			TEST_Component testComponent;
			testComponent.test();
		}
		{
			TEST_GameStateManager testGSM;
			testGSM.test();
		}
		{
			TEST_Object testObj;
			testObj.test();
		}
		string in = "";
		while (in != "y" && in != "n") {
			std::cout << "\n半自動テストを実行しますか？(y or n) :";
			std::cin >> in;
			std::this_thread::sleep_for(15ms);
		}
		if (in == "y") {
			{
				TEST_Camera testCamera;
				testCamera.test();
			}
		}
		else {
			std::cout << "\n半自動テストをスキップします\n";
		}

		std::cout << "\n((( TEST END )))" << std::endl;
		return true;
	}

	// コンソールの準備
	void ready_console() {
		::AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONIN$", "r", stdin);

		//s3d::Console.open();
	}
}
