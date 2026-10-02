#pragma once
#include "GameState.h"

/*
* ObjectとGSが相互インクルードしてたので用意したファイル
* ヘッダと実装を分離すれば大体は対処可能なのだが、今回はテンプレートで互いにアクセスがあった。
* .h と .cpp に分けるとしても、テンプレートはヘッダ側に書かねばならない。
* 
* 発生した問題を詳細に書こう。
* ここに定義してあるテンプレート関数を用意するにあたって、
* Object.h に GameState.h をインクルードする必要が生じた。
* しかし、GameState.h は Object.h をインクルードしているため、
* インクルード先で自分をインクルードしている、つまり相互インクルードの問題が発生したのだ。
* 当然エラーでビルドできなくなる。
* 先ほど書いたように、テンプレート関数はヘッダに書かねばならない。
* これが問題をやっかいにしていた。
* 利用先からテンプレートの実装が見えなければならないのだ。
* 
* ここでこのファイルの登場である。
* Object.h では関数の宣言だけをした。
* そして定義はこのファイルにある。
* GameState.h はここでインクルードしているため、Object.h からインクルードする必要はなくなった。
* そして、このヘッダを GameState.h ファイルの最後でインクルードするようにしたのだ。
* これにより、GameState.h をインクルードしてさえいればここの関数が使える。
* 構造としては微妙な気もするが、一応コンパイルは通った。
* どのコンパイル単位においても、変な問題は生じなかったということらしい。
* 正直理解はイマイチなので、どっかのタイミングでもっと良い実装を見つけたい気はする。
*/

namespace SGM2 {

	template<typename T>
	shared_ptr<T> Object::add_child(shared_ptr<T> obj, const int32 layerNo_) {
		if (obj != nullptr) {
			if (gameState == nullptr)
				throw std::runtime_error("add_childをコンストラクタからコールしている可能性があります。ready関数を利用してください。");
			obj->gameState = gameState;
			obj->init_layer_no(layerNo_);
			obj->parent = this;
			children.push_back(obj);
			childrenIsAdded = true;
			gameState->set_child_to_camera(obj, layerNo_);
			return obj;
		}
		return nullptr;
	}

	template<typename T>
	shared_ptr<T> Object::add_child(T* obj, const int32 layerNo_) {
		return add_child(shared_ptr<T>(obj), layerNo_);
	}
}
