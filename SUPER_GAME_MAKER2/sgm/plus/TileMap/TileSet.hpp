#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"
#include "TileLayer.h"
#include "Tile.hpp"

namespace SGM2::plus::tile_map {

	class TileSet {

		//inline static const std::unordered_map<string, Tile::Attribute> typeNameList = {
		//	{ "", Tile::Attribute::None },
		//	{ "hit", Tile::Attribute::Hit },
		//	{ "up", Tile::Attribute::UpHit },
		//	{ "right", Tile::Attribute::RightHit },
		//	{ "down", Tile::Attribute::DownHit },
		//	{ "left", Tile::Attribute::LeftHit }
		//};

	public:

		struct MetaData {
			Size tileSize;
			Size imageSize;
			uint32 columns;
			string name;
			uint32 firstId;
			uint32 lastId;	// この値は含む。ここまでってこと。
		};

		TileSet(const JSON& json, const vector<weak_ptr<TileLayer>>& layers) :
			tiles(std::make_unique<unordered_map<uint32, unique_ptr<Tile>>>()),
			tileSetMetaData(std::make_unique<unordered_map<string, MetaData>>())
		{

			// すべてのタイルセットデータを走査
			// タイルセットデータには特別なプロパティを持つものしか含まれない
			for (const auto& [_, tileSetJson] : json) {

				// データの読み込み
				// 名前じゃなくてメタデータを返せばよさそうだが、メモリ上の位置が変わったら困るのでこれで
				// unordered_mapのメモリ配置イマイチわかってないんだ...
				string tileSetName = load_meta_data(tileSetJson);

				// 特殊タイルを取得
				load_special_tiles(tileSetJson, tileSetName);
			}
			// 通常タイルの読み込み
			load_normal_tiles(layers);
			
		}

		const unordered_map<uint32, unique_ptr<Tile>>& get_tiles() const { return *tiles; }

		void update() {
			for (const auto& [_, tile] : *tiles) {
				tile->update();
			}
			Print << Unicode::Widen(std::format("Tile Num: {}", tiles->size()));
		}

	private:
		string load_meta_data(const JSON& tileSetJson) {
			MetaData data;
			data.tileSize.x = tileSetJson[U"tilewidth"].get<int32>();
			data.tileSize.y = tileSetJson[U"tileheight"].get<int32>();
			data.imageSize.x = tileSetJson[U"imagewidth"].get<int32>();
			data.imageSize.y = tileSetJson[U"imageheight"].get<int32>();
			data.columns = tileSetJson[U"columns"].get<int32>();
			data.name = tileSetJson[U"name"].getUTF8String();
			data.firstId = tileSetJson[U"firstgid"].get<int32>();
			data.lastId = tileSetJson[U"tilecount"].get<int32>() + data.firstId - 1;
			(*tileSetMetaData)[data.name] = data;
			return data.name;
		}

		void load_special_tiles(const JSON& tileSetJson, const string& tileSetName) {

			const MetaData& data = tileSetMetaData->at(tileSetName);

			// 各タイルのデータを取得
			for (const auto& [_, tileJson] : tileSetJson[U"tiles"]) {

				const uint32 idxInTileSet = tileJson[U"id"].get<int32>();
				const uint32 idxInLayer = idxInTileSet + data.firstId;
				const uint32 row = idxInTileSet / data.columns;
				const uint32 col = idxInTileSet - row * data.columns;

				// タイルデータの内容を走査
				for (const auto& [idx, value] : tileJson) {

					// アニメーションが設定されていた場合
					if (idx.toUTF8() == "animation") {

						(*tiles)[idxInLayer] = std::make_unique<AnimationTile>(idxInLayer);
						AnimationTile* tile = (AnimationTile*)tiles->at(idxInLayer).get();

						// フレームごとに情報を取得
						for (const auto& [_, v] : value) {

							const double duration = v[U"duration"].get<int32>() / 1000.0;
							const uint32 tileId = v[U"tileid"].get<int32>();

							const uint32 row = tileId / data.columns;
							const uint32 col = tileId - row * data.columns;

							tile->add_frame({
								TextureAsset(Unicode::Widen(data.name))(col * data.tileSize.x, row * data.tileSize.y, data.tileSize.x, data.tileSize.y),
								duration
							});
						}
					}

					// カスタムプロパティが設定されていた場合
					//if (idx.toUTF8() == "properties") {
					//	for (const auto& [_, v] : value) {

					//		// 属性タイルだった場合
					//		if (v[U"name"].getUTF8String() == "attribute") {

					//			const Tile::Attribute type = typeNameList.at(v[U"value"].getUTF8String());

					//			if (tiles->contains(idxInLayer)) {
					//				tiles->at(idxInTileSet)->set_attribute(type);
					//			}
					//			else {
					//				(*tiles)[idxInLayer] = std::make_unique<NormalTile>(
					//					idxInLayer,
					//					TextureAsset(Unicode::Widen(data.name))(col * data.tileSize.x, row * data.tileSize.y, data.tileSize.x, data.tileSize.y),
					//					type
					//				);
					//			}
					//		}
					//	}
					//}
				}
			}
		}

		void load_normal_tiles(const vector<weak_ptr<TileLayer>>& layers) {

			for (const auto& layer : layers) {
				for (const auto& line : layer.lock()->data) {
					for (const uint32 id : line) {

						// すでに登録済みか 0 ならスルー
						if (id == 0 || tiles->contains(id))
							continue;

						// そのタイルに関係するメタデータを取得
						const MetaData* metaData = nullptr;
						for (const auto& [_, mData] : *tileSetMetaData) {
							if (id >= mData.firstId && id <= mData.lastId) {
								metaData = &mData;
								break;
							}
						}
						if (metaData == nullptr)
							throw std::runtime_error(std::format("タイルセットに存在しないIDがレイヤー内に存在します！ id: {}", id));

						// タイルデータ生成
						const uint32 idInTileSet = id - metaData->firstId;
						const uint32 row = idInTileSet / metaData->columns;
						const uint32 col = idInTileSet - row * metaData->columns;
						(*tiles)[id] = std::make_unique<NormalTile>(
							id,
							TextureAsset(Unicode::Widen(metaData->name))(col * metaData->tileSize.x, row * metaData->tileSize.y, metaData->tileSize.x, metaData->tileSize.y)
						);
					}
				}
			}
		}

		// スライシングを防ぐためにunique_ptr<Tile>を使っている
		// unordered_mapはラップしないとエラーになった
		// 理由は不明
		unique_ptr<unordered_map<uint32, unique_ptr<Tile>>> tiles;
		unique_ptr<unordered_map<string, MetaData>> tileSetMetaData;
	};
}

