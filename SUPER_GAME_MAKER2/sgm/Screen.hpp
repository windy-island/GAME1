#pragma once

#include "std.h"
#include "Siv3Dplus.h"

namespace SGM2 {

	/**
	 * @brief スクリーン
	 * 
	 * @par 詳細
	 * 各オブジェクトはカメラを介してここに描画される。
	 * <br><br>
	 * 内部にレンダーテクスチャを保持しており、カメラがこれに描画。
	 * それをスクリーン位置に描画するというのが基本的な処理の流れ。
	 * 
	 * @note
	 * 枠線とか拡張して定義できるようにしたい。
	 * <br><br>
	 * 位置やサイズを変更する関数も用意すること。
	 */
	class Screen {

	public:
		/**
		 * @brief コンストラクタ
		 * 
		 * @par 詳細
		 * 位置やサイズ、カメラIDを指定する。指定されたIDのカメラがソースとなる。
		 * 
		 * @param pos_ 位置
		 * @param size_ サイズ
		 * @param cameraId_ カメラID
		 */
		Screen(const Vec2I& pos_, const Size& size_, const uint32 cameraId_ = 1) :
			pos(pos_),
			size(size_),
			cameraId(cameraId_),
			renderTexture(size_, Color(0, 0)),
			renderTextureMS(size_, Color(0, 0)) {
		}

		/**
		 * @brief ソースとなるカメラのIDを指定
		 */
		void set_camera_id(const uint32 cameraId_) { cameraId = cameraId_; }

		/**
		* @brief カメラのIDを取得
		*/
		uint32 get_camera_id() const { return cameraId; }

		/**
		* @brief 位置を取得
		*/
		const Vec2I& get_pos() const { return pos; }

		/**
		* @brief サイズを取得
		*/
		const Size& get_size() const { return size; }

		/**
		* @brief スクリーン座標系における、スクリーン中央の座標値を取得
		*/
		const Vec2I& get_center_posS() const { return { size.x / 2, size.y / 2 }; }

		/**
		* @brief サイズの半分を取得
		*/
		const Size& get_half_size() const { return { size.x / 2, size.y / 2 }; }

		/**
		* @brief デバイス座標系（クライアント領域内の座標系）における、スクリーン中央の座標値を取得
		*/
		const Vec2I& get_center_posD() const { return pos + get_center_posS(); }

	private:
		Vec2I pos;
		Size size;
		uint32 cameraId;
	public:
		RenderTexture renderTexture;
		MSRenderTexture renderTextureMS;
	};
}
