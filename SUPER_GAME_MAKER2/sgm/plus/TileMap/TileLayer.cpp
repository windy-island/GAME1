
#include "TileLayer.h"
#include "TileSet.hpp"

namespace SGM2::plus::tile_map {

	void TileLayer::draw(const DrawingProperties& dp) const {
		if (tileSet == nullptr)
			throw std::runtime_error("タイルレイヤーにタイルセットが渡っていません");

		if (!visible)
			return;

		// タイルマップ内における、スクリーンの左上座標を取得
		Vec2 screenTopLeftW = dp.camera.get_pos().xy() - dp.screen.get_half_size();
		Vec2 screenTopLeftInMap = screenTopLeftW - get_pos().xy();

		// マップデータ上の描画開始Index
		const Vec2I startIdx{
			(int32)(screenTopLeftInMap.x / tileSize.x),
			(int32)(screenTopLeftInMap.y / tileSize.y)
		};

		// 描画タイルの枚数（縦横）
		const Size size{
			(int32)(dp.screen.get_size().x / tileSize.x) + 1,
			(int32)(dp.screen.get_size().y / tileSize.y) + 1
		};

		Vec2I tileIdx{ 0, 0 };

		// 行を走査
		for (int i = 0; i < size.y; ++i) {
			tileIdx.y = startIdx.y + i;
			if (tileIdx.y < 0 || tileIdx.y >= data.size())
				continue;
			const auto& line = data.at(tileIdx.y);

			// 列を走査
			for (int j = 0; j < size.x; ++j) {
				tileIdx.x = startIdx.x + j;
				if (tileIdx.x < 0 || tileIdx.x >= line.size())
					continue;
				const uint32 id = line.at(tileIdx.x);

				// タイル描画
				if (id != 0)
					tileSet->get_tiles().at(id)->draw(dp.pos + tileIdx * tileSize * dp.scale, opacity, dp.scale);
			}
		}
		Print << Unicode::Widen(name) << U".z : " << get_global_pos().z;
	}
}
