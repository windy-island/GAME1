#pragma once

#include "sgmPlus.h"
#include "Text.hpp"

class TextObjA : public Object {

public:
	TextObjA() :
		Object({ 0, 0, 0 }),
		text()
	{}

private:
	ui::Text text;

	void ready() override {
		text.set_text("みなさんこんにちは！Hello World！\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		text.play();
	}

	bool update() override {
		text.advance_time(milliseconds((int)(Scene::DeltaTime() * 1000.0)));
		return true;
	}

	void draw(const DrawingProperties& dp) const override {
		text.draw(FontAsset(U"B"), { dp.screen.get_pos() + Vec2I{ 500, 200 }, {500, 200} }, { 255, 255, 255 });
	}
};
