#pragma once

#include "sgm.h"
#include "Siv3Dplus.h"
#include "TileMapCollider.hpp"

namespace SGM2::plus::tile_map {

	using SGM2::plus::hitbox::collider::SimpleTileLayerData;

	class TileSet;

	/*
	* hitboxのTileMapColliderに依存している箇所がある
	*/
	class TileLayer : public Object {

	public:
		vector<vector<uint32>> data;
		uint32 layerId;
		Size size;
		Size tileSize;
		string name;
		double opacity;
		bool visible;
		const TileSet* tileSet;
		

		TileLayer(const JSON& layerJson, const Size& tileSize_) :
			Object({ 0, 0, 0 }),
			tileSize(tileSize_),
			tileSet(nullptr)
		{
			// 諸々のデータ取得
			size.x = layerJson[U"width"].get<int32>();
			size.y = layerJson[U"height"].get<int32>();
			layerId = layerJson[U"id"].get<uint32>();
			name = layerJson[U"name"].getUTF8String();
			opacity = layerJson[U"opacity"].get<double>();
			visible = layerJson[U"visible"].get<bool>();

			// z値指定があれば取得
			if (layerJson[U"properties"]) {
				for (const auto& [_, value] : layerJson[U"properties"]) {
					if (value[U"name"].getUTF8String() == "z" || value[U"name"].getUTF8String() == "Z") {
						pos.z = value[U"value"].get<int32>();
					}
				}
			}
			
			// マップデータ分の領域を確保
			data.reserve(size.y);
			for (int i = 0; i < size.y; ++i)
				data.emplace_back(size.x);

			// マップデータを取得
			int x = 0;
			int y = 0;
			for (const auto& [index, value] : layerJson[U"data"]) {

				data.at(y).at(x) = value.get<int32>();
				++x;

				if (x >= size.x) {
					x = 0;
					++y;
				}
			}
		}

		void set_tile_set(const TileSet* tileSet_) { tileSet = tileSet_; }

		SimpleTileLayerData get_simple_tile_layer_data() const {
			std::cout << data.at(0).at(0) << std::endl;
			return { data, tileSize };
		}

		const string& get_name() const { return name; }

	private:
		void draw(const DrawingProperties& dp) const override;
	};
}
