#include "sgmPlus.h"
#include "GS_Startup.hpp"
#include "Test.hpp"

// SGM2 Ver2.01 (途中) を利用しています
// sgmフォルダの中身を更新し、プロジェクトに追加し直すことでバージョンをアップしてください

#ifdef _DEBUG
#pragma comment(lib, "../SUPER_GAME_MAKER2/App/SUPER_GAME_MAKER2(debug).lib")
#else
#pragma comment(lib, "../SUPER_GAME_MAKER2/App/SUPER_GAME_MAKER2.lib")
#endif

constexpr int32 SCREEN_W = 480 * 3;
constexpr int32 SCREEN_H = 320 * 3;

#define TEST_MODE

void Main() {
	try {

		Window::Resize(SCREEN_W, SCREEN_H);
		Scene::SetBackground(Color(0, 0, 0));

		// アンチエイリアスをオフ
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		Scene::SetTextureFilter(TextureFilter::Nearest);

		// テストを実行
#ifdef TEST_MODE
		SGM2_TEST::test();
#endif

		gsm.reset(GameStateManager::create_instance(new GS_Startup()));
		Stopwatch stopwatch;

		while (System::Update() && gsm) {
			stopwatch.restart();
			ClearPrint();
			Print << U"FPS: " << (int)(1.0 / Scene::DeltaTime() + 0.5);
			if (!gsm->update())
				break;
			Print << U"{0: >3}"_fmt(stopwatch.ms()) << U" ms";
		}
		return;
	}
	catch (const std::exception& e) {
		System::MessageBoxOK(U"exception", Unicode::Widen(e.what()), MessageBoxStyle::Error);
		throw;
	}
	return;
}
