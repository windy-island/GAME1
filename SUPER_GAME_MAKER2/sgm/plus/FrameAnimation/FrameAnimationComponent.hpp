#pragma once

#include "Siv3Dplus.h"
#include "std.h"
#include "Component.hpp"
#include "Clip.hpp"

namespace SGM2::plus::frame_animation {

	class FrameAnimationComponent : public Component {

	public:

		FrameAnimationComponent() :
			animations(),
			sppedRatio(1.0),
			isPlay(false),
			nowClipName(""),
			flipH(false),
			flipV(false) {
		}

		// アニメーションクリップを追加
		void add_animation_clip(const string& name, const bool isLoop_ = false) {
			animations[name] = Clip(isLoop_);
		}

		// 再生中はできない
		void add_frame_to_clip(const string& name, const Frame& frame) {
			try {
				if (!isPlay)
					animations.at(name).add_frame(frame);
			}
			catch (const std::exception& e) {
				throw std::runtime_error("存在しないアニメーションクリップにフレームを追加しました。先にクリップを作成してください。");
			}
		}

		// アニメーションが止まるので注意
		void delete_animation_clip(const string& name) {
			stop();
			std::erase_if(animations, [name](const auto& v) {
				return v.first == name;
			});
		}

		string play(const string& name) {
			try {
				nowClipName = name;
				isPlay = true;
				animations.at(name).start();
			}
			catch (const std::exception& e) {
				throw std::runtime_error("存在しないアニメーションクリップを再生しようとしています。指定したクリップ名を見直してください。");
			}
			return name;
		}

		void stop() {
			isPlay = false;
		}

		void restart() {
			isPlay = true;
		}

		void set_playback_speed(const double ratio_) { sppedRatio = ratio_; }

		void set_flip_horizontal(const bool v) { flipH = v; }

		void set_flip_vertical(const bool v) { flipV = v; }
		

		bool is_finished() const {
			return animations.at(nowClipName).is_finished();
		}

		int32 get_frame_index() const {
			animations.at(nowClipName).get_frame_index();
		}

		string get_clip_name() const { return nowClipName; }

	private:
		void ready() override {}

		// 描画するフレームの更新とか
		void update() override {
			if (nowClipName == "")
				return;

			auto& clip = animations.at(nowClipName);
			clip.update(Scene::DeltaTime() * sppedRatio, !isPlay);
		}

		void draw(const DrawingProperties& dp) const override {
			if (nowClipName == "")
				return;

			auto& clip = animations.at(nowClipName);
			clip.draw(dp, flipH, flipV);
		}

		unordered_map<string, Clip> animations;
		double sppedRatio;
		bool isPlay;
		string nowClipName;
		bool flipH;
		bool flipV;
	};
}
