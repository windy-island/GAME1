#pragma once

#include "sgmPlus.h"
#include "GS_Main.hpp"

class GS_Startup : public GameState {

public:

	GS_Startup() {
		// ゲーム初期化処理
		load_images();
		load_fonts();
		Print.setFont(FontAsset(U"B"));
	}

private:

	void ready() override {
		// ゲームシーンへ遷移
		gsm->reserve_transition(*this, new GS_Main());
	}

	void update() override {}

	void draw() const override {}



	// 画像をロード
	void load_images() const {
		TextureAsset::Register(U"MAIN2", U"./Item/image/MAIN2.png");
		TextureAsset::Register(U"attribute", U"./Item/image/attribute.png");
		TextureAsset::Register(U"lake", U"./Item/image/lake.png");
		TextureAsset::Load(U"MAIN2");
		TextureAsset::Load(U"attribute");
		TextureAsset::Load(U"lake");
	}



	// フォントをロード
	void load_fonts() const {
		FontAsset::Register(U"A", FontMethod::Bitmap, 12, U"item/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
		FontAsset::Register(U"B", FontMethod::Bitmap, 24, U"item/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
		FontAsset::Register(U"C", FontMethod::Bitmap, 36, U"item/font/PixelMplus/PixelMplus12-Regular.ttf", FontStyle::Bitmap);
		FontAsset::Load(U"A");
		FontAsset::Load(U"B");
		FontAsset::Load(U"C");
	}
};

