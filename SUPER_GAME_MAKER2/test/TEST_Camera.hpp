#pragma once

#include "sgmPlus.h"
#include "TestMacro.h"
#include "3D_A/Line.hpp"
#include "3D_A/Triangle.hpp"
#include "3D_A/Quad.hpp"

namespace SGM2_TEST {

	/*************************************************************** オブジェクト類 ***************************************************************/

	class Ball : public Object {

	public:
		Ball(const Vec3& pos_) : Object(pos_) {}

	private:
		void ready() override {}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override {
			Circle(dp.pos, 2 * dp.scale).draw(Palette::Blue);
		}
	};




	class Shell : public Object {

	public:
		Shell(const double t_) :
			Object({ 0, 0, 0 }),
			t(t_) {}

	private:
		double t;

		void ready() override {}

		bool update() override {
			t += Scene::DeltaTime();
			pos = Vec3(100 * cos(t * 3), 0, 100 * sin(t * 3));
			return true;
		}

		void draw(const DrawingProperties& dp) const override {
			Circle(dp.pos, 15 * dp.scale).draw(Palette::Purple);
		}
	};

	class Core : public Object {

	public:
		Core(const Vec3& pos_) :
			Object(pos_),
			t(0.0),
			basePos(pos_),
			image(TextureAsset(U"logo")) {
		}

	private:
		double t;
		const Vec3 basePos;
		const Texture image;

		void ready() override {
			add_child(new Shell(PI * 0.0));
			add_child(new Shell(PI * 0.5));
			add_child(new Shell(PI * 1.0));
			add_child(new Shell(PI * 1.5));
		}

		bool update() override {
			t += Scene::DeltaTime();
			pos = basePos + Vec3{ 200 * sin(t / 2), 100 * cos(t / 2), 0};
			return true;
		}

		void draw(const DrawingProperties& dp) const override {
			//Circle(dp.pos, 10 * dp.scale).draw(Palette::Orange);
			image.scaled(dp.scale).drawAt(dp.pos);
			FontAsset(U"A")(U"AAAA333\nこんにちは").drawAt(100, 101, Palette::Black);
			FontAsset(U"A")(U"AAAA333\nこんにちは").drawAt(101, 101, Palette::Black);
			FontAsset(U"A")(U"AAAA333\nこんにちは").drawAt(100, 100);
		}
	};

	class Background : public Object {

	public:
		Background() :
			Object(),
			bg(TextureAsset(U"mountain")),
			bgScale(Scene::Size().x / bg.size().x * 2),
			bgSize(bg.size().x * bgScale)
		{
			config.mustDraw = true;
			config.needsDrawingProperties = false;
		}

	private:
		const Texture bg;
		double bgScale;
		int bgSize;

		void ready() override {}

		bool update() override { return true; }

		void draw(const DrawingProperties& dp) const override {
			// ● バイリニア補間で背景を拡大描画
			const auto& camera = (const A3::Camera3D&)dp.camera;
			const ScopedRenderStates2D sampler(SamplerState::RepeatLinear);
			double rotationY = camera.get_angle().y;
			double rotationX = camera.get_angle().x;
			Vec2 baseDest(
				dp.screen.get_pos().x - rotationY * 1500,
				dp.screen.get_pos().y + rotationX * 500
			);
			Vec2 dest = baseDest;
			// 流石に後で直せよ
			while (dest.x - bgSize / 2 > Scene::Size().x)
				dest.x -= bgSize;
			while (dest.x + bgSize / 2 < 0)
				dest.x += bgSize;

			// ここも直す。描画は2つでいいはずだ。
			bg.scaled(bgScale).drawAt(dest.x, dest.y);
			bg.scaled(bgScale).drawAt(dest.x - bgSize, dest.y);
			bg.scaled(bgScale).drawAt(dest.x + bgSize, dest.y);
		}
	};


	class CameraObj : public Object {

	public:
		CameraObj(const A3::Camera3D& camera_) : Object(), camera(camera_) {}

	private:
		const A3::Camera3D& camera;

		void ready() override {}

		bool update() override {
			pos = camera.get_pos();
			return true;
		}

		void draw(const DrawingProperties& dp) const override {
			Circle(dp.pos, 20 * dp.scale).draw(Palette::Red);
		}
	};


	/*************************************************************** ゲームステート ***************************************************************/

	class GS_CameraTest1 : public GameState {
	public:
		GS_CameraTest1() :
			font(FontMethod::Bitmap, 12, U"item/font/PixelMplus/PixelMplus12-Regular.ttf"),
			camera(*add_camera(new A3::Camera3D({ 0, -30, 0 }, 1.0))),
			camera2(*add_camera(new A3::Camera3D({ 0, -300, -1000 }, 0.4))),
			screen(*add_screen(new Screen({ 300, 300 }, { 600, 450 }))),
			screen2(*add_screen(new Screen({ 0, 100 }, { 350, 300 }))),
			screen3(*add_screen(new Screen({ 380, 0 }, { 600, 300 }, camera2.get_camera_id())))
			//screen(*add_screen(new Screen({ 0, 0 }, { Scene::Size()})))
		{
			camera.config.isRetro = false;
			camera2.config.isRetro = false;

			for (int i = -5; i <= 5; ++i)
				for (int j = -5; j <= 5; ++j)
					add_object(new Ball({ i * 100, 0, j * 100 }));
			add_object(new Ball({ 0, -500, 0 }));
			add_object(new Ball({ 0, 500, 0 }));

			add_object(new Core({ 0, -250, 500 }));
			add_object(new A3::Line({ -500, 0, -500 }, { -500, 0, 500 }), 2);
			add_object(new A3::Line({ -400, 0, -500 }, { -400, 0, 500 }), 2);
			add_object(new A3::Line({ -500, 0, 400 }, { 500, 0, 400 }), 2);
			add_object(new A3::Triangle({ -500, 0, -500 }, { -500, 0, 500 }, { -800, -300, 0 }, Palette::Silver), 5);
			add_object(new A3::Quad({ 500, 0, -500 }, { 500, 0, 500 }, { 500, -300, 500 }, { 500, -300, -500 }), 5);
			add_object(new A3::Quad({ 500, 0, 500 }, { 500, 0, -500 }, { -500, 0, -500 }, { -500, 0, 500 }, Palette::Gray), 5);

			add_object(new Background(), 10);
			add_object(new CameraObj(camera));

			camera2.rotate_x(-PI / 12);
		}

	private:
		Font font;
		A3::Camera3D& camera;
		A3::Camera3D& camera2;
		Screen& screen;
		Screen& screen2;
		Screen& screen3;

		void ready() override {
		}

		void update() override {
			if (KeyQ.down())
				gsm->reserve_transition(*this, nullptr, true);

			int speed = 300;
			// カメラ向き
			Vec2 centerToCursor = Cursor::Pos() - screen.get_center_posD();
			if (centerToCursor.length() > 30) {
				camera.rotate_y(centerToCursor.x / 30000);
				camera.rotate_x(-centerToCursor.y / 30000);
				if (camera.get_angle().x > PI * 0.4)
					camera.set_angle_x(PI * 0.4);
				if (camera.get_angle().x < -PI * 0.4)
					camera.set_angle_x(-PI * 0.4);
			}

			// カメラ移動
			Vec3 forward = Vec::project_onto_xz(camera.forward()).normalized();
			Vec3 right = Vec::project_onto_xz(camera.right()).normalized();
			if (KeyW.pressed() || KeyUp.pressed()) camera.move(forward * speed * Scene::DeltaTime());
			if (KeyD.pressed() || KeyRight.pressed()) camera.move(right * speed * Scene::DeltaTime());
			if (KeyS.pressed() || KeyDown.pressed()) camera.move(-forward * speed * Scene::DeltaTime());
			if (KeyA.pressed() || KeyLeft.pressed()) camera.move(-right * speed * Scene::DeltaTime());
		}

		void draw() const override {
			// ● 文字
			font(U"Cameraテスト").drawAt(Scene::Size().x / 2, 30).asRect().scaled(1.2, 1.0).draw(Palette::Black);
			font(U"Cameraテスト").drawAt(Scene::Size().x / 2, 30);
		}
	};

	/*************************************************************** テスト ***************************************************************/

	class TEST_Camera {

	public:
		bool test() {
			TEST_ROOT_HEADER("Camera");

			gsm.reset(GameStateManager::create_instance(new GS_CameraTest1()));
			Stopwatch stopwatch;
			while (System::Update() && gsm) {
				stopwatch.restart();
				ClearPrint();
				Print << U"FPS: " << (int)(1.0 / Scene::DeltaTime() + 0.5);
				if (!gsm->update())
					break;
				Print << U"{0: >3}"_fmt(stopwatch.ms()) << U" ms";
			}

			//TEST_main_functionality();

			TEST_ROOT_FOOTER;
		}

	private:
		// 主要機能のチェック
		//bool TEST_main_functionality() {
		//	TEST_FUNC_HEADER;

		//	// ● オブジェクトのaddができるか。readyとupdateが正常に動作するか。
		//	SHOULD_TRUE({
		//		gsm.reset(GameStateManager::create_instance(new GS_8()));
		//	for (int i = 0; i < 3; ++i)
		//		gsm->update();
		//		}, (num == 16), "基本機能（add, ready, update）に不具合あり", "予期せぬ例外");

		//	TEST_FUNC_FOOTER("主要機能のチェック（GSへのadd、readyやupdateの動作確認など）");
		//}
	};
}
