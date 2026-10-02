
// ヘッダファイルの読み込み順は非常に重要
// 逆にするとエラーになる
// GSにあるget_objectでは、Objectのget_childをコールしており、どちらもテンプレート
// おそらくだが、逆にするとObjectのget_childでオーバーロード的なのがちゃんとできないのだろう
// なんでうまくいかないのかはやっぱり謎なんだけど
#include "GameState.h"
#include "Object.h"

int SGM2::Object::nextId = Object::FIRST_OBJECT_ID;

int SGM2::Object::get_game_state_id() const { return gameState->get_id(); }
