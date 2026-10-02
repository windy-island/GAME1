/*****************************************************************//**
 * @file   Text.hpp
 * @brief  
 * 
 * @author Y
 * @date   2026/9
 *********************************************************************/
#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"

/*
* 内部で経過時間と表示文字数を保持している。
* わざわざ2つの変数を用意したのは、途中で文字送りの速度を変更できるようにするため。
*/

namespace SGM2::plus::ui {

	/**
	 * @brief
	 * 矩形領域に1文字ずつテキストを出力する。
	 * 
	 * @par 詳細
	 */
	class Text {

	private:
		//Rect rect;				/// 描画位置とサイズ
		string content;			/// 出力テキスト
		milliseconds interval;	/// 文字送りの
		milliseconds time;		/// 前回も文字出力以後の経過時間
		int32 charCount;		/// 出力する文字の数
		bool isPlaying;			/// 再生中かどうか
		bool isFinished;		/// テキストを全て出力し終えたかどうか

	public:
		Text() :
			//rect(pos_, size_),
			content(""),
			interval(30ms),
			time(0),
			isPlaying(false),
			isFinished(false){
		}

		//void set_pos(const Vec2I& pos_) { rect.x = pos_.x; rect.y = pos_.y; }
		//void set_size(const Size& size_) { rect.w = size_.x; rect.h = size_.y; }

		void set_text(const string& text) {
			content = text;
			reset();
		}

		/**
		 * @brief
		 * 文字送り間隔を指定
		 * 
		 * @param ms
		 * 文字送り間隔をミリ秒で指定
		 * 
		 * @par 詳細
		 * ここで設定した時間が経過するごとに1文字出力される。
		 */
		void set_interval(const milliseconds ms) { interval = ms; }

		/**
		 * @brief
		 * 文字出力か完了したかどうか。
		 */
		bool is_finished() const { return isFinished; }

		/**
		 * @brief
		 * 経過時間や出力文字数を初期化して停止。
		 */
		void reset() {
			charCount = 0;
			time = 0ms;
			isFinished = false;
			stop();
		}

		/**
		 * @brief
		 * 再生。時間経過を受け付けるようになる。
		 * 
		 * @par 詳細
		 * 停止中は時間経過が発生しない。経過時間を指定しても無視される。
		 */
		void play() { isPlaying = true; }

		/**
		 * @brief
		 * 停止。時間経過を無視させる。
		 */
		void stop() { isPlaying = false; }

		/**
		 * @brief
		 * 時間を進める
		 * 
		 * @par 詳細
		 * 本クラスは時間経過によって文字送りを進めるが、時間計測の機能自体は持っていない。
		 * この関数で進めたい時間を渡す。
		 * <br>
		 * 再生中のみ値を受け付け、停止中は無視する。
		 * <br>
		 * 時間経過に応じて、出力文字数を制御。
		 */
		void advance_time(const milliseconds ms) {
			if (!isPlaying || isFinished) {
				return;
			}
			time += ms;
			while (time > interval && charCount < content.size()) {
				time -= interval;
				charCount += 1;
			}
			if (charCount >= content.size()) {
				isFinished = true;
			}
		}

		/**
		 * @brief
		 * 描画
		 * 
		 * @par 詳細
		 * 描画位置や範囲はここで渡す。メンバとして保持しない。<br>
		 * SGM2のObject::draw()やComponent::draw()はconstな関数である。
		 * したがって、描画位置をメンバにするとdrawから位置指定を行えなくなってしまうのだ。<br>
		 * こういった情報は引数として渡す必要がある。
		 */
		void draw(const Font& font, const Rect& rect, const Color color = { 0, 0, 0 }) const {
			//String key = Unicode::Widen(fontKey);
			//if (fontKey == "") {
			//	throw std::runtime_error("フォントがセットされていません！");
			//}
			//if (!FontAsset(key)) {
			//	throw std::runtime_error("フォントが無効です！");
			//}
			//FontAsset(key)(Unicode::Widen(content.substr(0, charCount))).draw(rect, color);
			font(Unicode::Widen(content.substr(0, charCount))).draw(rect, color);
		}
		void draw(const Font& font, const Vec2I& pos, const Size& size, const Color color = { 0, 0, 0 }) const {
			draw(font, Rect(pos, size), color);
		}
	};

}
