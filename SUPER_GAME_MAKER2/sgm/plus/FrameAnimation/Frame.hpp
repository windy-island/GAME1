#pragma once

#include "Siv3Dplus.h"
#include "std.h"

namespace SGM2::plus::frame_animation {

	struct Frame {
		Frame(const TextureRegion& image_, const double duration_, const Vec2I& offset_ = { 0, 0 }) :
			image(image_),
			duration(duration_),
			offset(offset_) {
		}

		TextureRegion image;
		double duration;
		Vec2I offset;
	};
}
