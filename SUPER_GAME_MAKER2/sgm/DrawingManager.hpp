#pragma once

#include "std.h"
#include "CameraBase.hpp"
#include "Screen.hpp"

namespace SGM2 {

	class GameState;

	/**
	 * @brief DM: 描画系統を管理するクラス
	 * 
	 * @par 詳細
	 * 各GSに1つずつ配置される。
	 * <br><br>
	 * カメラやスクリーン、描画用のオブジェクト配列を保持。
	 * <br><br>
	 * メンバのほとんどは非公開であり、GS等を介して操作することになる。
	 */
	class DrawingManager {

		static constexpr uint32 FIRST_CAMERA_ID = 1;

		friend class GameState;

	public:
		/**
		 * @brief コンストラクタ
		 */
		DrawingManager() :
			objectsFlatLayers(),
			cameras(),
			screens(),
			nextCameraId(FIRST_CAMERA_ID),
			shouldSort(false) {
		}

		/**
		 * @brief 描画レイヤーの数を取得
		 */
		int32 get_layer_count() const { return (int32)objectsFlatLayers.size(); }

		/**
		 * @brief カメラにソートを実行させる
		 */
		void reserve_sort() { shouldSort = true; }

	private:
		template<typename T>
		shared_ptr<T> add_camera(shared_ptr<T> camera) {
			camera->cameraId = nextCameraId;
			++nextCameraId;
			cameras.push_back(camera);
			return camera;
		}

		//template<typename T>
		//shared_ptr<T> add_camera(T* camera) {
		//	return add_camera(shared_ptr<T>(camera));
		//}

		shared_ptr<Screen> add_screen(shared_ptr<Screen> screen) {
			screens.push_back(screen);
			return screen;
		}

		//shared_ptr<Screen> add_screen(Screen* screen) {
		//	return add_screen(shared_ptr<Screen>(screen));
		//}

		// カメラに指示を出して描画させる
		void trigger_draw() {

			delete_destroyed_obj();

			for (auto& camera : cameras) {
				camera->trigger_draw(objectsFlatLayers, screens, shouldSort);
			}
			shouldSort = false;
		}

		// 破棄されたオブジェクトを削除
		void delete_destroyed_obj() {
			for (auto& layer : objectsFlatLayers) {
				auto& objectsFlat = layer.second;
				for (int i = (int)objectsFlat.size() - 1; i >= 0; --i) {
					if (objectsFlat.at(i).expired())
						objectsFlat.erase(objectsFlat.begin() + i);
				}
			}
		}

		// GSから呼ぶ
		void add_object(const weak_ptr<Object> object, const int layerNo = 0) {
			objectsFlatLayers[-layerNo].push_back(object);
			shouldSort = true;
		}

		// オブジェクトは親子構造を分解したフラットな状態で保持される
		// これらはレイヤーごとに分けて管理され、レイヤー番号の小さいものが手前に表示される
		// 現状では複数カメラの場合でも自身の属するレイヤー番号を変更できない。
		// これはオブジェクトがレイヤー番号を保持しているのと、レイヤーを後から変更できない点に起因する。
		// 問題があれば改良するとよい。
		// 
		// mapの要素はポインタにしといた方が処理効率いいかな
		// 要素を順番に並べているとしたら、ベクトルの要素数変動でコピーが大量発生しそうだもんね
		// ポインタの配列ならそんなことは起こらない
		std::map<int32, vector<weak_ptr<Object>>> objectsFlatLayers;
		vector<shared_ptr<CameraBase>> cameras;
		vector<shared_ptr<Screen>> screens;
		uint32 nextCameraId;
		bool shouldSort;
	};
}
