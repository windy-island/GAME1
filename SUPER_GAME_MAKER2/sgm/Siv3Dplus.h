#pragma once

#include <Siv3D.hpp>

using Vec2I = Point;
using Vec3I = Point3D;

// 透過レンダーテクスチャを扱うのに必要
// この関数自体はリファレンス的なのから持ってきた
BlendState make_blend_state();
