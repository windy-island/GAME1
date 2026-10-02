#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"

namespace SGM2::plus::tile_map {

	class Tile {

	public:
		//enum class Attribute {
		//	None,
		//	Hit,
		//	UpHit,
		//	RightHit,
		//	DownHit,
		//	LeftHit
		//};

		Tile(const uint32 id_) : id(id_) {
		}

		virtual ~Tile() = default;

		virtual void update() {}

		virtual void draw(const Vec2& pos, const double& alpha, const int32 scale) = 0;

	protected:
		const uint32 id;
	};



	class NormalTile : public Tile {

	public:
		NormalTile(const uint32 id_, const TextureRegion& image_) :
			Tile(id_),
			image(image_) {
		}

		void draw(const Vec2& pos, const double& alpha, const int32 scale) override {
			image.scaled(scale).draw(pos, AlphaF(alpha));
		}

	protected:
		const TextureRegion image;
	};



	class AnimationTile : public Tile {

	public:

		struct Frame {
			TextureRegion image;
			double duration;
		};

		AnimationTile(const uint32 id_) :
			Tile(id_),
			frames(),
			frameIdx(0),
			time(0) {
		}

		void add_frame(const Frame& frame) {
			frames.push_back(frame);
		}

		void update() override {
			time += Scene::DeltaTime();
		}

		void draw(const Vec2& pos, const double& alpha, const int32 scale) override {
			const auto& frame = frames[frameIdx];
			frame.image.scaled(scale).draw(pos, AlphaF(alpha));
			if (time > frame.duration) {
				time -= frame.duration;
				++frameIdx;
				if (frameIdx >= frames.size())
					frameIdx = 0;
			}
		}

	private:
		vector<Frame> frames;
		int32 frameIdx;
		double time;
	};


	//class AttributeTile : public NormalTile {

	//public:
	//	enum class Attribute {
	//		None,
	//		Hit,
	//		UpHit,
	//		RightHit,
	//		DownHit,
	//		LeftHit
	//	};

	//	AttributeTile(const uint32 id_, const TextureRegion& image_, const Attribute type_ = Attribute::None) :
	//		NormalTile(id_, image_),
	//		type(type_) {
	//	}

	//	/*void draw(const Vec2& pos) override {
	//		image.draw(pos, AlphaF(0.7));
	//	}*/

	//private:
	//	const Attribute type;
	//};
}
