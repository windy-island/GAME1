
#include "catch2.hpp"
#include "sgmPlus.h"
#include "./TestData.hpp"

namespace SGM2_TEST::GameStateManager {

	/*
	* ### 要テスト
	*
	* + GS::updateからの遷移
	* + Obj::updateからの遷移
	* + コンポーネントからの遷移
	* + オーバーライド版が同じように動くか
	* + プッシュのみの遷移
	* + ポップのみの遷移
	* + ポップしてからプッシュ
	*
	* ### 機能として必要なのか微妙な辺り
	*
	* + トップ以外からの遷移要求
	*/
	TEST_CASE("GameStateManager", "[base]") {
		gsm.reset();
		num = 0;



		/********************************************************************************************************************************************************************/

		SECTION("インスタンスの生成") {

			SECTION("初期ステートなしでちゃんと例外を出すか") {
				CHECK_THROWS([]() {
					unique_ptr<SGM2::GameStateManager> pGSM(SGM2::GameStateManager::create_instance(nullptr));
				}());
			}

			SECTION("正しくGSMが生成できるか") {
				CHECK_NOTHROW([]() {
					unique_ptr<SGM2::GameStateManager> pGSM;
					pGSM.reset(SGM2::GameStateManager::create_instance(new GS_A()));
				}());
			}

			SECTION("2つ目のインスタンス生成で例外を出すか（GSMはシングルトン）") {
				CHECK_THROWS([]() {
					unique_ptr<SGM2::GameStateManager> pGSM1, pGSM2;
					pGSM1.reset(SGM2::GameStateManager::create_instance(new GS_A()));
					pGSM2.reset(SGM2::GameStateManager::create_instance(new GS_A()));
				}());
			}

			/*
			* 下記は
			* pGSM.reset(SGM2::GameStateManager::create_instance(new GS_A()));
			* pGSM.reset(SGM2::GameStateManager::create_instance(new GS_A()));
			* だと例外がとぶ。
			* resetよりも先に引数内のcreateが実行されるため。
			*/
			SECTION("1つ目のインスタンス破棄後に、2つ目のインスタンスを生成可能か（GSMはシングルトン）") {
				CHECK_NOTHROW([]() {
					unique_ptr<SGM2::GameStateManager> pGSM;
					pGSM.reset(SGM2::GameStateManager::create_instance(new GS_A()));
					pGSM.reset();
					pGSM.reset(SGM2::GameStateManager::create_instance(new GS_A()));
				}());
			}
		}



		/********************************************************************************************************************************************************************/

		SECTION("遷移予約、遷移") {

			/*
			* 1: 5
			* 2: 5 + 10 + 8 = 23
			* 3: 23 + 8 = 31
			*/
			SECTION("GSから遷移させられるか") {
				gsm.reset(SGM2::GameStateManager::create_instance(new GS_B1()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 31);
			}

			/*
			* 1: 0 + 3 = 3			[ GS_C1: 1, ObjC: 0 ]
			* 2: 3 + 4 + 3 = 10		[ GS_C1: 2, ObjC: 1 ] 遷移予約
			* 3: 10 + 10 + 8 = 28	[ GS_C2: 1 ]
			*/
			SECTION("Objectから遷移させられるか") {
				gsm.reset(SGM2::GameStateManager::create_instance(new GS_C1()));
				for (int i = 0; i < 3; ++i) {
					gsm->update();
				}
				CHECK(num == 28);
			}
		}
	}
}



