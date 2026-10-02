#pragma once

#include "sgmPlus.h"
#include "2D_A/Camera2D.hpp"


class Camera : public A2::Camera2D {

public:
	Camera(const Vec3& pos_, const double scale_, const Object& player_) :
		A2::Camera2D(pos_, scale_),
		player(player_) {
	}



private:
	// ******************************** // 変数 // ******************************** //
	const Object& player;



	// ************************* // 組み込みメンバ関数 // ************************* //

	void ready() override {}

	bool update() override {
		pos = player.get_global_pos();
		return true;
	}

	void draw(const DrawingProperties& dp) const override {}



	// ************************* // Privete メンバ関数 // ************************* //
};


