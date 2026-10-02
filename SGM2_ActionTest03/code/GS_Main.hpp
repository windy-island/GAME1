#pragma once

#include "sgmPlus.h"
#include "Global.h"
#include "Camera.hpp"
#include "TileMap.hpp"
#include "Character.hpp"
#include "ObjA.hpp"
#include "ObjB.hpp"
#include "ObjC.hpp"
#include "TextObjA.hpp"

class GS_Main : public GameState {
public:
	GS_Main() :
		cm(			*add_object(new CollisionManager())),
		character(	*add_object(new Character({ 900, 350, -3 }, cm))),
		camera(		*add_camera(new Camera({ 900, 350, 0 }, SCREEN_SCALE, character))),
		tileMap(	*add_object(new TileMap("./item/mapdata/mapA.json"))) {

		add_screen(new Screen({ 0, 0 }, Scene::Size()));
		tileMap.get_pos_ref() = { 100, 100, 0 };
	}



private:
	// ******************************** // 変数 // ******************************** //

	CollisionManager& cm;
	Character& character;
	A2::Camera2D& camera;
	TileMap& tileMap;




	// ************************* // 組み込みメンバ関数 // ************************* //

	void ready() override {
		add_object(new ObjA({ 1000, 330, 100 }, { 0, 0 }, cm));
		add_object(new ObjA({ 1200, 350, 100 }, { 10, 10 }, cm));
		add_object(new ObjB({ 900, 350, 100 }, { 10, 10 }, cm));
		add_object(new ObjC({ 1100, 350, 100 }, { 3, 0 }, cm));
		add_object(new ObjC({ 1100, 300, 100 }, { 3, 0 }, cm));
		add_object(new ObjC({ 1100, 250, 100 }, { 0, 0 }, cm));
		add_object(new TextObjA());

		if (auto layer = tileMap.get_tile_layer_by_name("Attribute")) {
			auto mapHitbox = tileMap.add_component(cm.create_hitbox(tileMap));
			mapHitbox->add_tile_map_collider_set(layer->get_simple_tile_layer_data());
		}
	}

	void update() override {

		// スローモーション
		if (KeyShift.pressed()) {
			std::this_thread::sleep_for(0.5s);
		}
	}

	void draw() const override {
		//TextureAsset(U"lake").scaled(SCREEN_SCALE).draw(0, 0);
		Rect{ {0, 0}, RESOLUTION * SCREEN_SCALE }.draw(Color(10, 139, 245));

		Print << U"HitBoxes num: " << cm.get_hitboxes_num();
	}



	// ************************* // Privete メンバ関数 // ************************* //
};
