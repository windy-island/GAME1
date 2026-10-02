#pragma once

#include "Camera3D.hpp"
#include "Triangle.hpp"

namespace SGM2::plus::A3 {

	class Quad : public Object {

	public:
		Quad (const array<Vec3, 4> vertices_, const Color& color_ = Palette::White) :
			Object(),
			vertices(vertices_),
			color(color_)
		{
			config.needsDrawingProperties = false;
			config.mustDraw = true;
			config.drawType = DrawType::Shape;
		}

		Quad (const Vec3& point0, const Vec3& point1, const Vec3& point2, const Vec3& point3, const Color& color_ = Palette::White) :
			Quad ({ point0, point1, point2, point3 }, color_) {
		}

	private:
		array<Vec3, 4> vertices;
		Color color;
		array<std::weak_ptr<A3::Triangle>, 2> triangles;

		void ready() override {
			triangles[0] = add_child(new A3::Triangle({ vertices[0], vertices[1], vertices[2] }, color), get_layer_no());
			triangles[1] = add_child(new A3::Triangle({ vertices[2], vertices[3], vertices[0] }, color), get_layer_no());
		}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override { }
	};
}
