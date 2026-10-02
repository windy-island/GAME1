#pragma once

#include "Camera3D.hpp"

namespace SGM2::plus::A3 {

	class Sphere : public Object {

	public:
		Sphere(const Vec3& pos_, const int32 radius_, const Color& color_) :
			Object(pos_),
			radius(radius_),
			color(color_) {
			config.drawType = DrawType::Shape;
		}

		void set_pos(const Vec3& pos_) {
			pos = pos_;
		}

	protected:
		int32 radius;
		Color color;

	private:
		void ready() override {}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override {
			Circle(dp.pos, radius * dp.scale).draw(color);
		}
	};
}
