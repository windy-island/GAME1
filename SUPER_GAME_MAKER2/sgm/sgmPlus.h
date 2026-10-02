#pragma once

#include "./Siv3Dplus.h" // Siv3D v0.6.14
#include "./sgm.h"
#include "./plus/HitBox/HitBoxComponent.h"
#include "./plus/HitBox/CollisionManager.h"
#include "./plus/Util/Vec.hpp"
#include "./plus/FrameAnimation/FrameAnimationComponent.hpp"
#include "./plus/TileMap/TileMap.hpp"
#include "./plus/TileMapEx/TileMapEx.hpp"
//#include "plus/3D_A/Camera3D.hpp"

using namespace SGM2;
using namespace SGM2::plus;
using namespace SGM2::plus::Utility;
using namespace SGM2::plus::frame_animation;
using namespace SGM2::plus::tile_map;
using namespace SGM2::plus::hitbox;
using namespace SGM2::plus::tile_map_ex;

// 乱数生成器
extern std::random_device seedGenerator;
extern std::mt19937_64 random;

// ゲームステートマネージャー
extern unique_ptr<SGM2::GameStateManager> gsm;
