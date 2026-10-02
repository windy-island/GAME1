#pragma once

#include "CameraBase.hpp"

namespace SGM2::plus::A2 {

	class Camera2D : public CameraBase {
	public:
		Camera2D(const Vec3& pos, const double scale_ = 1, const int32 priority_ = -INT_MAX) : CameraBase(pos, priority_), scale(scale_) {}

		const double get_scale() const { return scale; }

		void set_scale(const double scale_) { scale = scale_; }

	private:
		double scale;

		void sort_obj(vector<weak_ptr<Object>>& objectsFlat) const override {
			std::sort(objectsFlat.begin(), objectsFlat.end(), [](const auto& v0_, const auto& v1_) {
				const auto v0 = v0_.lock();
				const auto v1 = v1_.lock();
				return v0->get_global_pos().z > v1->get_global_pos().z;
			});
		}

		bool should_draw(const shared_ptr<const Object> obj) const override {
			return true;
		}

		// カメラ自身のposは中央座標
		// ワールド座標(0, 0)のオブジェクトは、カメラの位置が(0, 0)のときに画面中央へ描画される
		Vec2I calc_posS(const shared_ptr<const Object> obj, const Screen& screen) const override {
			return ((obj->get_global_pos() - pos).xy().asPoint() * scale + screen.get_half_size()).asPoint();
		}

		double calc_scale(const shared_ptr<const Object> obj) const override {
			return scale;
		}
	};
}
