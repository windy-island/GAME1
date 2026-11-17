#pragma once
#pragma once

#include "sgmPlus.h";

namespace SGM2_TEST::Manual::HitBox {

	class GS_HitBox : public GameState {

	public:
		GS_HitBox() {
			
		}

	private:
		void ready() override {}

		void update() override {
			if (KeyQ.down()) {
				gsm->reserve_transition(*this, nullptr, true);
			}
		}

		void draw() const override {
			FontAsset(U"M2")(U"HitBoxのテスト\nQキーで終了").drawAt(Scene::Width() / 2, Scene::Height() - 100, Palette::White);
		}
	};
}
