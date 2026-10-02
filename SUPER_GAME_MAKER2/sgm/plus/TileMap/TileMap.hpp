#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"
#include "TileLayer.h"
#include "TileSet.hpp"

namespace SGM2::plus::tile_map {

	// 使用ルール
	// ・属性タイルにはattributeプロパティを用意し、typeNameListに指定された文字列を与える。
	// ・画像名と同じ名前でTextureAssetに画像をロードしておく。
	// ・使用するタイルはすべて同じサイズにすること
	//		変えられるようにしてもいいけど、それはまた今度で
	// ・レイヤーに Zプロパティを用意すると反映される
	class TileMap : public Object {

		// 今のところ使ってない？
		//static constexpr string_view ATTRIBUTE_LAYER_NAME_0{ "Attribute" };
		//static constexpr string_view ATTRIBUTE_LAYER_NAME_1{ "attribute" };

	private:
		const string jsonPath;
		Size mapSize;
		Size tileSize;
		vector<weak_ptr<TileLayer>> layers;
		unique_ptr<TileSet> tileSet;

	public:
		TileMap(const string& mapDataPath) : Object(), jsonPath(mapDataPath) {
		}

	private:
		void ready() override {
			const JSON json = JSON::Load(Unicode::Widen(jsonPath));
			if (!json) {
				throw std::runtime_error("マップデータのロードに失敗！");
			}

			try {
				// マップデータの読み込み
				mapSize.x = json[U"width"].get<int32>();
				mapSize.y = json[U"height"].get<int32>();
				tileSize.x = json[U"tilewidth"].get<int32>();
				tileSize.y = json[U"tileheight"].get<int32>();

				// レイヤーデータの読み込み
				for (const auto& [layerIndex, layerJson] : json[U"layers"]) {
					layers.push_back(add_child(new TileLayer(layerJson, tileSize)));
					//layers.emplace_back(layerJson);
				}

				// タイルセットデータの読み込み
				tileSet = std::make_unique<TileSet>(json[U"tilesets"], layers);

				// レイヤーにタイルセットを渡す
				for (auto& layer : layers) {
					layer.lock()->set_tile_set(tileSet.get());
				}
			}
			catch (const std::exception& e) {
				System::MessageBoxOK(U"exception", Unicode::Widen(e.what()), MessageBoxStyle::Error);
				throw std::runtime_error("指定されたマップデータJSONの形式には対応していません！");
			}
			catch (...) {
				throw std::runtime_error("指定されたマップデータJSONの形式には対応していません！");
			}
		}

		bool update() override {
			tileSet->update();
			return true;
		}

		// tileMapのposはマップ左上の位置を表す
		void draw(const DrawingProperties& dp) const override {
		}

	public:

		// 名前を指定してレイヤーを取得
		// 取得失敗時には無効なポインタを返す
		// TileMapのreadyがコールされた後に呼び出すこと！
		const shared_ptr<TileLayer> get_tile_layer_by_name(const string& name) {
			for (auto wp : layers) {
				if (auto layer = wp.lock()) {
					std::cout << layer->get_name() << std::endl;
					if (layer->get_name() == name) {
						return layer;
					}
				}
			}
			return nullptr;
		}
	};
}
