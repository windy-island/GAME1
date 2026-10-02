#pragma once

#include "Siv3Dplus.h"

namespace SGM2::plus {

	namespace Utility {

		class Vec {

		public:
			// ベクトルをx軸周りで回転させる
			static Vec3 x_rotated(const Vec3& vec, const double radian) {
				return Vec3{
					vec.x,
					Math::Cos(radian) * vec.y - Math::Sin(radian) * vec.z,
					Math::Sin(radian) * vec.y + Math::Cos(radian) * vec.z
				};
			}

			// ベクトルをy軸周りで回転させる
			static Vec3 y_rotated(const Vec3& vec, const double radian) {
				return Vec3{
					Math::Cos(radian) * vec.x + Math::Sin(radian) * vec.z,
					vec.y,
					-Math::Sin(radian) * vec.x + Math::Cos(radian) * vec.z
				};
			}

			// ベクトルをz軸周りで回転させる
			static Vec3 z_rotated(const Vec3& vec, const double radian) {
				return Vec3{
					Math::Cos(radian) * vec.x - Math::Sin(radian) * vec.y,
					Math::Sin(radian) * vec.x + Math::Cos(radian) * vec.y,
					vec.z
				};
			}

			// 任意の軸周りで回転させる
			// ロドリゲスの回転公式と言うらしい
			static Vec3 rotated(const Vec3& vec, const double rad, const Vec3& axis) {
				const auto& x = axis.x;
				const auto& y = axis.y;
				const auto& z = axis.z;

				const double v = 1 - cos(rad);

				const double v11 = v * x * x + cos(rad);
				const double v12 = v * x * y - z * sin(rad);
				const double v13 = v * x * z + y * sin(rad);

				const double v21 = v * x * y + z * sin(rad);
				const double v22 = v * y * y + cos(rad);
				const double v23 = v * y * z - x * sin(rad);

				const double v31 = v * x * z - y * sin(rad);
				const double v32 = v * y * z + x * sin(rad);
				const double v33 = v * z * z + cos(rad);

				return Vec3{
					v11 * vec.x + v12 * vec.y + v13 * vec.z,
					v21 * vec.x + v22 * vec.y + v23 * vec.z,
					v31 * vec.x + v32 * vec.y + v33 * vec.z
				};
			}

			// XZ平面に投影
			static Vec3 project_onto_xz(const Vec3& vec) {
				return { vec.x, 0, vec.z };
			}

			// YZ平面に投影
			static Vec3 project_onto_yz(const Vec3& vec) {
				return { 0, vec.y, vec.z };
			}

			// XY平面に投影
			static Vec3 project_onto_xy(const Vec3& vec) {
				return { vec.x, vec.y, 0 };
			}

		private:
			Vec() = default;
		};
	}
}
