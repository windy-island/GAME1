#pragma once

#include "Frame.hpp"

namespace SGM2::plus::frame_animation {

	class Clip {

	public:

		Clip(const bool isLoop_ = false) :
			frames(),
			isLoop(isLoop_),
			frameIdx(0),
			time(0.0),
			isFinished(false) {
		}

		void add_frame(const Frame& frame) {
			frames.push_back(frame);
		}

		void start() {
			isFinished = false;
			frameIdx = 0;
			time = 0.0;
		}

		void update(const double deltaTime, const bool& isStop) {

			// 範囲チェック
			if (frameIdx < 0 || frameIdx >= frames.size())
				throw std::runtime_error("存在しないアニメーションフレームを再生しようとしています。");
			const auto& frame = frames.at(frameIdx);

			// アニメーション終了フラグが立っていない場合
			if (!isFinished) {

				// 指定時間を過ぎたらコマ送り
				if (!isStop)
					time += deltaTime;
				if (time > frame.duration) {
					time -= frame.duration;
					++frameIdx;
				}

				// 終了した場合
				if (frameIdx >= frames.size()) {
					if (isLoop)
						frameIdx = 0;
					else {
						isFinished = true;
						frameIdx = frames.size() - 1;
					}
				}
			}
		}

		void draw(const DrawingProperties& dp, const bool flipH = false, const bool flipV = false) const {

			// 範囲チェック
			if (frameIdx < 0 || frameIdx >= frames.size())
				throw std::runtime_error("存在しないアニメーションフレームを再生しようとしています。");

			// 描画
			const auto& frame = frames.at(frameIdx);
			const int32 h = flipH ? -1 : 1;
			const int32 v = flipV ? -1 : 1;
			frame.image
				.scaled(dp.scale * h, dp.scale * v)
				.drawAt(dp.pos + frame.offset * Vec2I{ h, v } * dp.scale);
		}

		int32 get_frame_index() const { return frameIdx; }

		bool is_finished() const { return isFinished; }

	private:
		vector<Frame> frames;
		bool isLoop;
		int32 frameIdx;
		double time;
		bool isFinished;
	};
}
