#pragma once

#include "./GS_Main.hpp"

namespace SGM2_TEST::Manual {

	constexpr int32 SCREEN_W = 480 * 3;
	constexpr int32 SCREEN_H = 320 * 3;

	void manual_test() {
		try {
			Window::Resize(SCREEN_W, SCREEN_H);
			Scene::SetBackground(Color(0, 0, 0));

			// アンチエイリアスをオフ
			const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
			Scene::SetTextureFilter(TextureFilter::Nearest);

			gsm.reset();
			gsm.reset(GameStateManager::create_instance(new SGM2_TEST::Manual::GS_Main()));
			Stopwatch stopwatch;

			while (System::Update() && gsm) {
				stopwatch.restart();
				ClearPrint();
				Print << U"FPS: " << (int)(1.0 / Scene::DeltaTime() + 0.5);
				if (!gsm->update()) { break; }
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
}
