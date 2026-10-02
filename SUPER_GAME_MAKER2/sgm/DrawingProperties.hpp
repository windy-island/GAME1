#pragma once

#include "std.h"
#include "Siv3Dplus.h"

namespace SGM2 {

	class Screen;
	class CameraBase;

	/**
	 * @brief DP: オブジェクトのdrawに渡される描画情報
	 * 
	 * @par 詳細
	 * オブジェクト設定をいじると、DPの計算を省略できたりする。
	 * <br>
	 * その場合でもカメラやスクリーンといった計算のいらない値は有効値が渡る。
	 */
	struct DrawingProperties {
		/**
		 * @brief 描画位置
		 * 
		 * @par 詳細
		 * 実は小数でもよかったのだが、ドット絵をきれいに出すためには整数でやる必要があった。
		 * <br>
		 * この値はスクリーン座標値である。
		 * オブジェクトの描画関数内ではスクリーン座標値で描画することになるため、そのまま扱ってよい。
		 */
		Vec2I pos = { 0, 0 };

		/**
		 * @brief スケール
		 */
		double scale = 1.0;

		/**
		* @brief 角度。ラジアンだった気がする。
		*/
		double angle = 0.0;

		/**
		* @brief 描画対象のスクリーン
		*/
		const Screen& screen;

		/**
		* @brief 使用されているカメラ
		*/
		const CameraBase& camera;
	};
}
