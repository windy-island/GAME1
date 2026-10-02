#pragma once

#include "Camera3D.hpp"

namespace SGM2::plus::A3 {

	class Line : public Object {

	public:
		Line(const array<Vec3, 2> vertices_, const Color& color_ = Palette::Black) :
			Object(),
			vertices(vertices_),
			color(color_) {
			config.needsDrawingProperties = false;
			config.mustDraw = true;
			config.drawType = DrawType::Shape;
		}

		Line(const Vec3& point0, const Vec3& point1, const Color& color_ = Palette::Black) :
			Line({ point0, point1 }, color_) {
		}

	private:
		array<Vec3, 2> vertices;
		Color color;

		void ready() override {}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override {
			// ビューポートの内外判定はカメラ任せにできない
			// 自身の位置は関係ないからだ
			// ここで判定をやっておく必要がある
			const A3::Camera3D& camera = (const Camera3D&)(dp.camera);
			if (camera.in_viewport(camera.world_to_camera_coordinates(vertices[0])) || camera.in_viewport(camera.world_to_camera_coordinates(vertices[1]))) {
				array<Vec2, 2> points = camera.world_to_screen_for_line(vertices, dp.screen);
				s3d::Line(points[0], points[1]).draw(color);
			}
		}
	};
}
