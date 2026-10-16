#pragma once

#include "sgmPlus.h";

namespace SGM2_TEST::Manual {

	class GS_ManualTestMain : public GameState {

	public:
		GS_ManualTestMain() {
			load_fonts();
		}

	private:
		void ready() override {}

		void update() override {
			if (KeyQ.down()) {
				gsm->reserve_transition(*this, nullptr, true);
			}
		}

		void draw() const override {
			FontAsset(U"M2")(U"マニュアルテストを行います\nQキーで終了").drawAt(Scene::Width() / 2, Scene::Height() - 100, Palette::White);
		}

		void load_fonts() const {
			FontAsset::Register(U"A", FontMethod::Bitmap, 12, U"items/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
			FontAsset::Register(U"B", FontMethod::Bitmap, 24, U"items/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
			FontAsset::Register(U"C", FontMethod::Bitmap, 36, U"items/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
			FontAsset::Register(U"M1", FontMethod::Bitmap, 24, U"items/font/BIZ_UDP_Mintyo/BIZUDPMincho-Regular.ttf", FontStyle::Bitmap);
			FontAsset::Register(U"M2", FontMethod::Bitmap, 36, U"items/font/BIZ_UDP_Mintyo/BIZUDPMincho-Regular.ttf", FontStyle::Bitmap);
			FontAsset::Load(U"A");
			FontAsset::Load(U"B");
			FontAsset::Load(U"C");
			FontAsset::Load(U"M1");
			FontAsset::Load(U"M2");
		}
	};
}
