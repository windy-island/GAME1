#pragma once

#include "sgmPlus.h";

namespace SGM2_TEST::Manual {

	class GS_Main: public GameState {

		const Texture bg;

	public:
		GS_Main() : bg(U"items/image/BG.JPG") {

			load_fonts();
			load_bgm();
		}

	private:
		void ready() override {
			AudioAsset(U"DebugTheme").play();
		}

		void update() override {
			if (KeyQ.down()) {
				gsm->reserve_transition(*this, nullptr, true);
			}
		}

		void draw() const override {
			bg.scaled(0.7).drawAt(Scene::Center());
			const auto textCenterPos = Vec2I(Scene::Width() / 2, Scene::Height() - 130);
			Rect(Arg::center(textCenterPos + Vec2I{ 0, 24 }), 470, 115).draw(Palette::Black);
			//FontAsset(U"M2")(U"マニュアルテストを行います\nQキーで終了").drawAt(Scene::Width() / 2, Scene::Height() - 100, Palette::White);
			FontAsset(U"M2")(U"マニュアルテストを行います").drawAt(textCenterPos, Palette::White);
			FontAsset(U"M2")(U"Qキーで終了").drawAt(textCenterPos + Vec2I{ 0, 48 }, Palette::White);
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

		void load_bgm() const {
			AudioAsset::Register(U"DebugTheme", U"items/bgm/town15.mp3", Loop::Yes);
			AudioAsset::Load(U"DebugTheme");
		}
	};
}
