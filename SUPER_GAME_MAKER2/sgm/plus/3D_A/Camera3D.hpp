#pragma once

#include "std.h"
#include "CameraBase.hpp"
#include "Vec.hpp"

namespace SGM2::plus::A3 {

	using namespace Utility;

	// 3D空間では、正面がZ軸正方向、右がX軸正方向、"下"がY軸正方向とする
	// 下がY軸正方向なので注意
	// 回転角は反時計回りが正なのだが、Y軸が下に伸びている関係上、パっと見では時計回りが正である

	// スクリーンをでかくしすぎると、見た目が若干おかしくなる
	// set_scaleで適度な画角に絞ること
	// 実際の3Dシステムでも画面端は伸びるでしょ
	// あれもちょっと変だけど、このシステムでは伸ばせないからよりおかしく見える
	// 1024*768 で scale=1.0 だとちょうどいいので、サイズに応じて倍率を調整すること

	// 手前と奥でクリッピングしているが、上下左右はやっていない
	class Camera3D : public CameraBase {

	public:
		static constexpr double DEFAULT_NEAR_PLANE = 20;//0.2;
		static constexpr double DEFAULT_FAR_PLANE = 33000;// 50.0;

		// Z値をそのまま使うとやたら影響が大きくなる
		// 小さくして使うとちょうどいい
		static constexpr double Z_RATE = 1000.0;

		static constexpr Vec3 WORLD_Z_AXIS{ 0, 0, 1.0 };

		Camera3D(const Vec3& pos_, const double scale_ = 1.0, const int32 priority_ = -INT_MAX, const double nearPlane_ = DEFAULT_NEAR_PLANE, const double farPlane_ = DEFAULT_FAR_PLANE) :
			CameraBase(pos_, priority_),
			angle(0, 0, 0),
			scaleW(scale_ * (Scene::Size().x / 1024.0)),	// スクリーンの影響を考慮
			nearPlane(nearPlane_),
			farPlane(farPlane_)
		{
			config.performSortEveryTime = true;
		}

		// ●●●●● // ●●●●● // ●●●●● // オーバーライド // ●●●●● // ●●●●● // ●●●●● //

		// オブジェクトをカメラからの距離でソートする
		void sort_obj(vector<weak_ptr<Object>>& objectsFlat) const override {
			std::sort(objectsFlat.begin(), objectsFlat.end(), [this](const auto& v0_, const auto& v1_) {
				const auto v0 = v0_.lock();
				const auto v1 = v1_.lock();
				if (v0 && v1) {
					auto posC0 = world_to_camera_coordinates(v0->get_global_pos());
					auto posC1 = world_to_camera_coordinates(v1->get_global_pos());
					double len0 = posC0.lengthSq();
					double len1 = posC1.lengthSq();

					if (len0 > len1)
						return true;
					else
						return false;
				}
				return false;
			});
		}

		bool should_draw(const shared_ptr<const Object> obj) const override {
			return in_viewport(world_to_camera_coordinates(obj->get_global_pos()));
		}

		Vec2I calc_posS(const shared_ptr<const Object> obj, const Screen& screen) const override {
			return world_to_screen_coordinates(obj->get_global_pos(), screen).asPoint();
		}

		double calc_scale(const shared_ptr<const Object> obj) const override {
			return calc_scale_from_posC(world_to_camera_coordinates(obj->get_global_pos()));
		}

		// ●●●●● // ●●●●● // ●●●●● // ●●●●● // ●●●●● // ●●●●● //


		Vec3 forward() const {
			return rotated_with_camera({ 0, 0, 1 });
		}

		Vec3 right() const {
			return rotated_with_camera({ 1, 0, 0 });
		}

		Vec3 up() const {
			return rotated_with_camera({ 0, 1, 0 });
		}

		// カメラの向きを考慮してカメラ座標を回転
		// カメラと逆に回転することで向きを調整する
		// 例えばカメラが右を向いている場合、物体は正面向きの場合に比べて左にずれる必要がある
		Vec3 rotated_inverse_of_camera(const Vec3& vec) const {

			// with_cameraではもう少し凝った回転をしているが、
			// こっちはこうやるだけの方がちゃんと動く
			// 理由は不明
			return Vec::x_rotated(Vec::y_rotated(vec, -angle.y), -angle.x);

			//auto yRotated = Vec::y_rotated(vec, -angle.y);
			//auto xAxisYrotated = Vec::y_rotated({ 1, 0, 0 }, -angle.y);
			//return Vec::rotated(yRotated, -angle.x, xAxisYrotated);
		}

		// カメラと同じ向きに回す
		Vec3 rotated_with_camera(const Vec3& vec) const {

			// これだとワールド座標系の軸で回転してしまう
			// return Vec::x_rotated(Vec::y_rotated(vec, angle.y), angle.x);

			// 最初のYはワールドの軸でいいが、次のXはY軸回転した後のX軸に対して回す必要がある
			auto yRotated = Vec::y_rotated(vec, angle.y);
			auto xAxisYrotated = Vec::y_rotated({ 1, 0, 0 }, angle.y);
			return Vec::rotated(yRotated, angle.x, xAxisYrotated);
		}

		// 座標変換
		// ワールド座標系 -> カメラ座標系
		// 最後に回転の計算をしているが、これをパスするとカメラの回転が無視される
		// その値を拡大率計算に使ってみたりすると、割とおもしろい挙動が見れたり
		Vec3 world_to_camera_coordinates(const Vec3& posInWorld) const {
			return rotated_inverse_of_camera(posInWorld - pos);
		}

		// 座標変換
		// カメラ座標系 -> スクリーン座標系
		Vec2 camera_to_screen_coordinates(const Vec3& cameraToObject, const Screen& screen) const {
			const Vec2& screenCenter = screen.get_center_posS();
			double z = cameraToObject.z > 0 ? cameraToObject.z : 0.00000001;
			//double z = cameraToObject.z != 0 ? cameraToObject.z : 0.00000001;
			z /= Z_RATE;
			return Vec2{
				//((cameraToObject.x / z) + screenCenter.x),
				//((cameraToObject.y / z) + screenCenter.y),
				(((cameraToObject.x * 1) / (z * 1)) * scaleW + screenCenter.x),
				(((cameraToObject.y * 1) / (z * 1)) * scaleW + screenCenter.y),
			};
		}

		// 座標変換
		// ワールド座標系 -> スクリーン座標系
		Vec2 world_to_screen_coordinates(const Vec3& posInWorld, const Screen& screen) const {
			return camera_to_screen_coordinates(world_to_camera_coordinates(posInWorld), screen);
		}

		// 線分の2点座標値を計算
		array<Vec2, 2> world_to_screen_for_line(const array<Vec3, 2>& pointsW, const Screen& screen) const {
			// ********** カメラ座標系に変換
			array<Vec3, 2> pointsC{
				world_to_camera_coordinates(pointsW[0]),
				world_to_camera_coordinates(pointsW[1])
			};

			// ********** 2点が両方とも同じ側の描画範囲外なら、テキトウな値を返して描画させない
			if ((pointsC[0].z < nearPlane && pointsC[1].z < nearPlane) || (pointsC[0].z > farPlane && pointsC[1].z > farPlane))
				return array<Vec2, 2>({ { -INT_MAX, -INT_MAX }, { -INT_MAX, -INT_MAX } });

			// ********** カメラの裏にいる場合が計算不可なので、その場合は「描画範囲前面」と線分の交点を求める
			// 手前の点と遠い点を求める（この段階では範囲外にいるとしても近い方だけ）
			Vec3* nearPoint = &pointsC[0];
			Vec3* farPoint = &pointsC[1];
			if (pointsC[0].z > pointsC[1].z) {
				nearPoint = &pointsC[1];;
				farPoint = &pointsC[0];;
			}

			// 手前の点が前面の裏にあった場合
			// ここの仕組みは notion に図説してある
			if (nearPoint->z < nearPlane) {
				double v0 = nearPlane - nearPoint->z;
				double v1 = farPoint->z - nearPlane;
				double sum = (v0 + v1) != 0 ? (v0 + v1) : 0.000001;
				double rate = v0 / sum;
				Vec3 nearToFar = *farPoint - *nearPoint;
				Vec3 nearToNearPlane = nearToFar * rate;
				*nearPoint = *nearPoint + nearToNearPlane;	// 線分と前面の交点で値を更新
			}

			// ********** あとは普通に計算
			array<Vec2, 2> result;
			for (int i = 0; i < 2; ++i)
				result[i] = camera_to_screen_coordinates(pointsC[i], screen);
			return result;
		}

		// 三角形の3点座標値を計算
		// しかし、1つの点が前面より手前に来た場合は四角を描画せねばならない
		// 無駄になる可能性もあるが、一応4つの点を返す
		array<Vec2, 4> world_to_screen_for_triangle(const array<Vec3, 3>& pointsW, const Screen& screen) const {
			// ********** カメラ座標系に変換
			array<Vec3, 3> pointsC{
				world_to_camera_coordinates(pointsW[0]),
				world_to_camera_coordinates(pointsW[1]),
				world_to_camera_coordinates(pointsW[2])
			};

			// ********** 近い順にソート
			std::sort(pointsC.begin(), pointsC.end(), [](const auto& v0, const auto& v1) { return v0.z < v1.z; });

			// ********** 最も奥の点が手前面より前なら、テキトウな値を返して描画させない
			if (pointsC[2].z < nearPlane)
				return array<Vec2, 4>({ { -INT_MAX, -INT_MAX }, { -INT_MAX, -INT_MAX }, { -INT_MAX, -INT_MAX }, { -INT_MAX, -INT_MAX } });

			array<Vec2, 4> result;
			// ********** 前面裏の値が1つか2つかで分岐
			if (pointsC[1].z < nearPlane) {
				// ********** 2つのケース「描画範囲前面」と線分の交点を求める
				// 最も遠い点から2つの点へ伸びる2本の線分を扱う
				// 描画するのは三角形となる
				for (int i = 0; i < 2; ++i) {
					Vec3* nearPoint = &pointsC[i];
					Vec3* farPoint = &pointsC[2];

					double v0 = nearPlane - nearPoint->z;
					double v1 = farPoint->z - nearPlane;
					double sum = (v0 + v1) != 0 ? (v0 + v1) : 0.000001;
					double rate = v0 / sum;
					Vec3 nearToFar = *farPoint - *nearPoint;
					Vec3 nearToNearPlane = nearToFar * rate;
					*nearPoint = *nearPoint + nearToNearPlane;	// 線分と前面の交点で値を更新
				}
				for (int i = 0; i < 3; ++i)
					result[i] = camera_to_screen_coordinates(pointsC[i], screen);
				result[3] = result[2];
			}
			else if (pointsC[0].z < nearPlane) {
				// ********** 1つのケース「描画範囲前面」と線分の交点を求める
				// 最も近い点から2つの点へ伸びる2本の線分を扱う
				// 描画するのは四角形となるため4つの点を求める
				array<Vec3, 4> points;
				for (int i = 0; i < 2; ++i) {
					Vec3* nearPoint = &pointsC[0];
					Vec3* farPoint = &pointsC[i + 1];

					double v0 = nearPlane - nearPoint->z;
					double v1 = farPoint->z - nearPlane;
					double sum = (v0 + v1) != 0 ? (v0 + v1) : 0.000001;
					double rate = v0 / sum;
					Vec3 nearToFar = *farPoint - *nearPoint;
					Vec3 nearToNearPlane = nearToFar * rate;
					if (i == 0) {
						points[0] = *nearPoint + nearToNearPlane;
						points[1] = *farPoint;
					}
					else {
						points[3] = *nearPoint + nearToNearPlane;
						points[2] = *farPoint;
					}
				}
				for (int i = 0; i < 4; ++i)
					result[i] = camera_to_screen_coordinates(points[i], screen);
			}
			else {
				// ********** 0つのケース 一番簡単
				for (int i = 0; i < 3; ++i)
					result[i] = camera_to_screen_coordinates(pointsC[i], screen);
				result[3] = result[2];
			}
			return result;
		}

		// 拡大率取得
		double calc_scale_from_posC(const Vec3& cameraToObject) const {
			double z = cameraToObject.z != 0 ? cameraToObject.z : 0.001;
			z /= Z_RATE;
			return (1.0 / z) * scaleW;
		}

		// ビューポートに入っているか
		// カメラ座標系で判定
		// ただし、一般的な判定とは少し違う
		// 前面より遠く、かつ、一定距離内ならビューポート内と判定している
		bool in_viewport(const Vec3& posFromCamera) const {
			return posFromCamera.z > nearPlane && posFromCamera.lengthSq() < farPlane * farPlane;
		}

		void move_forward(const double d) {
			pos += forward() * d;
		}
		void move_right(const double d) {
			pos += right() * d;
		}
		void move_up(const double d) {
			pos += up() * d;
		}

		void rotate_x(const double radian) {
			//direction = Vec::x_rotated(Vec::y_rotated({ 0, 0, 1 }, calc_directions_y_angle()), calc_directions_x_angle() + radian);
			//direction = rotated_with_camera(Vec::x_rotated({ 0, 0, 1 }, radian));
			//direction = Vec::x_rotated(direction, radian);
			angle.x += radian;
		}
		void rotate_y(const double radian) {
			//direction = Vec::x_rotated(Vec::y_rotated({ 0, 0, 1 }, calc_directions_y_angle() + radian), calc_directions_x_angle());
			//direction = rotated_with_camera(Vec::y_rotated({ 0, 0, 1 }, radian));
			//direction = Vec::y_rotated(direction, radian);
			angle.y += radian;
		}
		//void rotate_z(const double radian) { direction = Vec::z_rotated(direction, radian); }

		double get_x() const { return pos.x; }
		double get_y() const { return pos.y; }
		double get_z() const { return pos.z; }
		const Vec3& get_angle() const { return angle; }

		void set_angle_x(const double angleX) { angle.x = angleX; }
		void set_angle_y(const double angleY) { angle.y = angleY; }
		void set_angle_z(const double angleZ) { angle.z = angleZ; }
		void set_angle(const Vec3& angle_) { angle = angle_; }
		void set_scale(const double scale_) { scaleW = scale_; }
		void set_near_plane(const double v) { nearPlane = v; }
		void set_far_plane(const double v) { farPlane = v; }

	private:
		Vec3 angle;		// カメラの回転（XY軸回転のみ可能）
		double scaleW;	// 拡大率
		double nearPlane;
		double farPlane;
	};
}
