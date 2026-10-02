#pragma once

#include "Camera3D.hpp"

namespace SGM2::plus::A3 {

	class Triangle : public Object {

	public:
		Triangle (const array<Vec3, 3> vertices_, const Color& color_ = Palette::White) :
			Object(),
			vertices(vertices_),
			color(color_) {
			config.needsDrawingProperties = false;
			config.mustDraw = true;
			config.drawType = DrawType::Shape;
		}

		Triangle (const Vec3& point0, const Vec3& point1, const Vec3& point2, const Color& color_ = Palette::White) :
			Triangle ({ point0, point1, point2 }, color_) {
		}

	private:
		array<Vec3, 3> vertices;
		Color color;

		void ready() override {}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override {
			// ビューポートの内外判定はカメラ任せにできない
			// 自身の位置は関係ないからだ
			// ここで判定をやっておく必要がある
			const A3::Camera3D& camera = (const Camera3D&)(dp.camera);
			if (   camera.in_viewport(camera.world_to_camera_coordinates(vertices[0]))
				|| camera.in_viewport(camera.world_to_camera_coordinates(vertices[1]))
				|| camera.in_viewport(camera.world_to_camera_coordinates(vertices[2]))
				) {
				array<Vec2, 4> points = camera.world_to_screen_for_triangle(vertices, dp.screen);
				s3d::Triangle(points[0], points[1], points[2]).draw(color);
				s3d::Triangle(points[2], points[3], points[0]).draw(color);
			}
		}
	};
}
