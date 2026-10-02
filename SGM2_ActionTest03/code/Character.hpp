#pragma once

#include "sgmPlus.h"
#include "ObjB.hpp"
#include "ObjC.hpp"

class Character : public Object {

public:
	Character(const Vec3& pos_, CollisionManager& cm_) : Object(pos_), cm(cm_) {}



private:
	// ******************************** // 変数 // ******************************** //

	shared_ptr<FrameAnimationComponent> animation;
	CollisionManager& cm;
	shared_ptr<HitBoxComponent> hitbox;



	// ************************* // 組み込みメンバ関数 // ************************* //

	void ready() override {
		init_animation();
		hitbox = add_component(cm.create_hitbox(*this));
		auto& cset = hitbox->add_collider_set("body");
		cset.add_rect_collider({ 0, 0 }, { 12, 36 });

		hitbox->activate("body");
		cset.set_visible_flag(true);

		add_child(new ObjB({ 50, -50, 100 }, { 0, 0 }, cm));
		add_child(new ObjC({ -30, 0, 100 }, { 0, 0 }, cm));
	}

	bool update() override {
		// 現在のアニメーション取得
		string clip = animation->get_clip_name();

		// 当たり判定
		if (hitbox->is_hit()) {
			hitbox->set_debug_color(ColorF(1, 0, 0, 0.4));
		}
		else {
			hitbox->set_debug_color(ColorF(0, 1, 0, 0.4));
		}

		// アニメーション選択と操作
		if (clip == "attack" && animation->is_finished()) {
			clip = animation->play("idle");
		}

		if (clip == "idle" && clip != "walk") {
			if (KeyS.pressed() || KeyF.pressed() || KeyE.pressed() || KeyD.pressed()) {
				clip = animation->play("walk");
			}
		}

		if (clip == "idle" || clip == "walk") {
			if (KeyS.up() || KeyF.up() || KeyE.up() || KeyD.up()) {
				clip = animation->play("idle");
			}

			if (KeySpace.down()) {
				clip = animation->play("attack");
			}
		}

		if (clip == "walk") {
			if (KeyF.pressed()) {
				pos.x += 3;
				animation->set_flip_horizontal(true);
			}
			else if(KeyS.pressed()) {
				pos.x -= 3;
				animation->set_flip_horizontal(false);
			}
			else if (KeyE.pressed()) {
				pos.y -= 3;
			}
			else if (KeyD.pressed()) {
				pos.y += 3;
			}
		}
		return true;
	}

	void draw(const DrawingProperties& dp) const override {
		Print << U"Player Pos: " << pos;
		Print << U"CollisionTileData: " << hitbox->get_sub_collision_dataA();
	}



	// ************************* // Privete メンバ関数 // ************************* //

	void init_animation() {
		animation = add_component(new FrameAnimationComponent());

		// 待機
		animation->add_animation_clip("idle", true);
		animation->add_frame_to_clip("idle", Frame(
			TextureAsset(U"MAIN2")(0, 336, 32, 48),
			1.0
		));

		// 移動
		animation->add_animation_clip("walk", true);
		for (int i = 0; i < 10; ++i) {
			animation->add_frame_to_clip("walk", Frame(
				TextureAsset(U"MAIN2")(32 + 32 * i, 384, 32, 48),
				0.07
			));
		}

		// 攻撃
		animation->add_animation_clip("attack", false);
		animation->add_frame_to_clip("attack", Frame(
			TextureAsset(U"MAIN2")(0, 432, 32, 48),
			0.1
		));
		animation->add_frame_to_clip("attack", Frame(
			TextureAsset(U"MAIN2")(224, 432, 64, 48),
			0.1,
			{ -16, 0 }
		));
		animation->add_frame_to_clip("attack", Frame(
			TextureAsset(U"MAIN2")(416, 432, 32, 48),
			0.1
		));

		animation->play("idle");
	}
};

