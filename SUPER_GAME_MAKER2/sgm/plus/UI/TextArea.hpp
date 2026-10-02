//#pragma once
//
//#include "sgm.h"
//#include "Siv3Dplus.h"
//
//namespace SGM2::plus::ui {
//
//	class TextArea : public Object {
//
//	private:
//		string textBuffer;
//		milliseconds normalInterval;
//		milliseconds fastInterval;
//		Size size;
//		unordered_map<string, Font> fonts;
//		Font* usingFont;
//
//	public:
//		TextArea(const Vec3& pos_) :
//			Object(pos_),
//			textBuffer(""),
//			normalInterval(30ms),
//			fastInterval(5ms),
//			size(300, 100),
//			fonts(),
//			usingFont(nullptr) {
//			fonts["default"] = Font(FontMethod::MSDF, 16, Typeface::Regular);
//			usingFont = &fonts["default"];
//		}
//
//		void set_text(const string& text) { textBuffer = text; }
//
//		void set_normal_interval(const milliseconds ms) { normalInterval = ms; }
//
//		void set_fast_interval(const milliseconds ms) { fastInterval = ms; }
//
//		void set_size(const Size& size_) { size = size_; }
//
//		void clear_fonts() noexcept { fonts.clear(); }
//
//		void add_font(const string& key, const Font& font) { fonts[key] = font; }
//
//		void select_font(const string& fontName) {
//			if (fonts.contains(fontName)) {
//				usingFont = &fonts[fontName];
//			}
//			throw std::runtime_error("存在しないフォントのキーを選択しました。");
//		}
//
//		/**
//		 * @brief
//		 * 文字出力を開始。停止中なら再開する。
//		 */
//		void play() const {
//
//		}
//
//	private:
//		void ready() override {}
//
//		bool update() override { return true; }
//
//		void draw(const DrawingProperties& dp) const override {}
//	};
//
//}
