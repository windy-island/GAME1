#pragma once

#include "std.h"
#include "Screen.hpp"
#include "Object.h"

namespace SGM2 {

	class GameState;
	class DrawingManager;

	/**
	 * @brief カメラを作成する際の基底クラス
	 * 
	 * @par 詳細
	 * このクラスを拡張して自由にカメラを作成することが可能。
	 * というか、このクラス自体は抽象クラスなので拡張しないと使えない。
	 * SGM2側ではいくつかの拡張カメラをデフォルトで提供している。
	 * <br><br>
	 * オブジェクトの draw() に渡される引数はカメラクラスで計算されている。
	 * これらを定義するには拡張用関数をオーバーライドすること。
	 * <br><br>
	 * オブジェクトの重なりはソート関数のオーバーライドで定義できるが、それ以外にもレイヤーで管理する方法がある。
	 * 
	 * @note
	 * スクリーン、レイヤー、オブジェクト単位で任意のカスタムシェーダーを実行できるようにしたい。
	 */
	class CameraBase : public Object {

		friend class GameState;
		friend class DrawingManager;

		struct ObjectData {
			float objX = 0;
			float objY = 0;
			float objZ = 0;
		};

		struct TimeData {
			float time = 0;
		};

		struct CameraData {
			float CamX = 0;
			float CamY = 0;
			float CamZ = 0;
		};

	public:

		enum class ShaderMode {
			None,
			Screen,
			Layer,
			Object
		};

		/**
		 * @brief カメラの設定
		 */
		struct Config {
			/**
			 * @brief ソート処理が毎フレームコールされるようになる
			 * 
			 * @par 詳細
			 * 2Dのカメラにおいて、ソート処理を行う必要があるのはオブジェクトが追加されたときだけである。
			 * よってデフォルトの動作はそういうケースでのみソートするようになっている。
			 * <br>
			 * しかし、3Dカメラにおいては毎フレームソートが必要だろう。
			 * そういうケースではこの設定をいじる。
			 */
			bool performSortEveryTime = false;

			/**
			 * @brief マルチサンプル・エイリアシングをオフ。
			 * 
			 * @par 詳細
			 * オフにするとジャギーが出てレトロ感が出る。
			 * 高解像度ゲームなら設定を変更しよう。
			 */
			bool isRetro = true;

			/**
			 * @brief シェーダーモード
			 * 
			 * @par 詳細
			 * 今度書く
			 */
			ShaderMode shaderMode = ShaderMode::None;
		};

		Config config;
		//Vec3 pos;
	private:
		uint32 cameraId;
		//RenderTexture renderTexture;
		//MSRenderTexture renderTextureMS;
		weak_ptr<vector<Screen>> screens;
		VertexShader vertexShader2D;
		PixelShader pixelShader2DShape;
		PixelShader pixelShader2DTexture;
		ConstantBuffer<ObjectData> cbObject;
		ConstantBuffer<CameraData> cbCamera;
		ConstantBuffer<TimeData> cbTime;

	public:
		/**
		 * @brief コンストラクタ
		 * @param pos_ カメラの位置
		 */
		CameraBase(const Vec3& pos_, const int32 priority_ = -INT_MAX) :
			Object(pos_, priority_),
			config(),
			//pos(pos_),
			cameraId(0),
			screens()
		{
			// デフォルトシェーダーのロード
			vertexShader2D = HLSL{ U"example/shader/hlsl/default2d.hlsl", U"VS" } | GLSL{ U"example/shader/glsl/default2d.vert", {{U"VSConstants2D", 0}} };
			pixelShader2DShape = HLSL{ U"example/shader/hlsl/default2d.hlsl", U"PS_Shape" } | GLSL{ U"example/shader/glsl/default2d_shape.frag", {{U"PSConstants2D", 0}} };
			pixelShader2DTexture = HLSL{ U"example/shader/hlsl/default2d.hlsl", U"PS_Texture" } | GLSL{ U"example/shader/glsl/default2d_texture.frag", {{U"PSConstants2D", 0}} };

			if (!vertexShader2D || !pixelShader2DShape || !pixelShader2DTexture) {
				throw std::runtime_error("カメラにてSiv3Dデフォルトシェーダーの読み込みに失敗しました");
			}
		}

		virtual ~CameraBase() = default;

		/**
		 * @brief カメラIDを取得。マルチカメラなら必須。
		 */
		uint32 get_camera_id() const { return cameraId; }

		///**
		//* @brief カメラの位置を取得
		//*/
		//const Vec3& get_pos() const { return pos; }

		/**
		* @brief 位置を指定
		*/
		void set_pos(const Vec3& pos_) { pos = pos_; }

		/**
		* @brief 指定したベクトル分だけ動かす
		*/
		void move(const Vec3& move) { pos += move; }	// ワールド座標値での移動

		/**
		* @brief 位置のX成分を動かす
		*/
		void add_x(const double x) { pos.x += x; }

		/**
		* @brief 位置のY成分を動かす
		*/
		void add_y(const double y) { pos.y += y; }

		/**
		* @brief 位置のZ成分を動かす
		*/
		void add_z(const double z) { pos.z += z; }

		/**
		* @brief 図形用ピクセルシェーダーをセット
		*/
		void set_pixel_shader_shape(const PixelShader& pixelShader) {
			pixelShader2DShape = pixelShader;
		}

		/**
		* @brief テクスチャ用ピクセルシェーダーをセット
		*/
		void set_pixel_shader_texture(const PixelShader& pixelShader) {
			pixelShader2DTexture = pixelShader;
		}
		
	private:

		// オブジェクトを描画する
		void trigger_draw(std::map<int32, vector<weak_ptr<Object>>>& objectsFlatLayers, vector<shared_ptr<Screen>>& screens, const bool isSort) {
			cbTime->time += (float)Scene::DeltaTime();
			cbCamera->CamX = (float)pos.x;
			cbCamera->CamY = (float)pos.y;
			cbCamera->CamZ = (float)pos.z;

			if (!screens.empty()) {
				// ソート
				if (isSort || config.performSortEveryTime)
					for (auto& layer : objectsFlatLayers) {
						std::cout << "layer id: " << layer.first << std::endl;
						sort_obj(layer.second);
					}

				// 描画
				for (const auto& screen : screens) {
					if (config.isRetro) {
						// レンダーテクスチャを使うことで、マルチサンプル・アンチエイリアシングをオフにできる
						// これで図形描画にもジャギーが入り、レトロ感が増す
						if (cameraId == screen->get_camera_id()) {
							{
								const ScopedRenderTarget2D target{ screen->renderTexture.clear(Color(0, 0)) };
								const ScopedRenderStates2D blend{ make_blend_state() };
								draw(objectsFlatLayers, *screen);
							}
							screen->renderTexture.draw(screen->get_pos());
						}
					}
					else {
						// ちゃんとアンチエイリアスをかけるならこっち
						// ただ、背景が黒のテクスチャとして処理するため、場合によっては描画対象のふちが黒っぽくなってしまう
						// 背景もオブジェクトで用意してれば問題ないが、GSで描いてる場合は前景に縁取りがつくかも
						if (cameraId == screen->get_camera_id()) {
							{
								const ScopedRenderTarget2D target{ screen->renderTextureMS.clear(Color(0, 0)) };
								const ScopedRenderStates2D blend{ make_blend_state() };
								draw(objectsFlatLayers, *screen);
								Graphics2D::Flush();
							}
							screen->renderTextureMS.resolve();
							screen->renderTextureMS.draw(screen->get_pos());
						}
					}
				}
			}
		}

		// 描画
		void draw(const std::map<int32, vector<weak_ptr<Object>>>& objectsFlatLayers, const Screen& screen) {
			for (const auto& layer : objectsFlatLayers) {
				for (const auto& object : layer.second) {
					if (const auto& obj = object.lock())
						if (obj->is_sorted() && (obj->config.mustDraw || should_draw(obj))) {

							// DPの準備
							unique_ptr<DrawingProperties> dp;
							if (obj->get_config().needsDrawingProperties)
								dp = std::make_unique<DrawingProperties>(calc_posS(obj, screen), calc_scale(obj), 0.0, screen, *this);
							else
								dp = std::make_unique<DrawingProperties>(Vec2I{ 0, 0 }, 1.0, 0.0, screen, *this);

							// シェーダーの適用
							const auto& objPos = obj->get_global_pos();
							cbObject->objX = (float)objPos.x;
							cbObject->objY = (float)objPos.y;
							cbObject->objZ = (float)objPos.z;

							if (config.shaderMode == ShaderMode::Object) {
								if (obj->get_config().drawType == DrawType::Texture) {
									Graphics2D::SetPSConstantBuffer(1, cbTime);
									Graphics2D::SetPSConstantBuffer(2, cbObject);
									Graphics2D::SetPSConstantBuffer(3, cbCamera);
									const ScopedCustomShader2D shader{ vertexShader2D, pixelShader2DTexture };
									obj->trigger_draw(*dp);
								}
								else {
									Graphics2D::SetPSConstantBuffer(1, cbTime);
									Graphics2D::SetPSConstantBuffer(2, cbObject);
									Graphics2D::SetPSConstantBuffer(3, cbCamera);
									const ScopedCustomShader2D shader{ vertexShader2D, pixelShader2DShape };
									obj->trigger_draw(*dp);
								}
							}
							else {
								obj->trigger_draw(*dp);
							}
						}
				}
			}
		}

		// ● // ● // ● // ユーザー拡張関数 // ● // ● // ● //

		/**
		 * @brief ソート関数（オーバーライド必須）
		 * 
		 * @par 詳細
		 * オブジェクトをソートし、描画する順番を指定する。これによって前後関係が制御される。
		 * <br>
		 * 渡されるオブジェクト配列は、親子構造を分解して平坦化した配列。
		 * <br><br>
		 * レイヤーごとにコールされるため、レイヤー内での描画順を指定するということになる。
		 * 
		 * @param objectsFlat オブジェクト配列
		 */
		virtual void sort_obj(vector<weak_ptr<Object>>& objectsFlat) const = 0;

		/**
		 * @brief 描画するかどうか（オーバーライド必須）
		 * 
		 * @par 詳細
		 * 各オブジェクトごとにコールされる。falseを返すと描画されない。
		 * <br><br>
		 * ビューポート内ならtrue、外ならfalseを返せばよいだろう。
		 * 
		 * @param obj オブジェクト
		 * @return 描画フラグ
		 */
		virtual bool should_draw(const shared_ptr<const Object> obj) const = 0;

		/**
		 * @brief 描画位置を計算（オーバーライド必須）
		 * 
		 * @par 詳細
		 * 各オブジェクトごとにコールされる。
		 * <br><br>
		 * オブジェクトの描画位置（スクリーン座標値）を計算して返す。
		 * 
		 * @param obj オブジェクト
		 * @param screen 対象のスクリーン
		 * @return 描画位置
		 */
		virtual Vec2I calc_posS(const shared_ptr<const Object> obj, const Screen& screen) const = 0;

		/**
		 * @brief スケールを計算（オーバーライド必須）
		 * 
		 * @par 詳細
		 * 各オブジェクトごとにコールされる。
		 * 
		 * @param obj オブジェクト
		 * @return スケール
		 */
		virtual double calc_scale(const shared_ptr<const Object> obj) const = 0;
	};
}
