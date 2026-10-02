#include "SAMPLE_GameState.h"

namespace SGM2_TEST {

	int32 num = 0;

	// 初回更新時に遷移を予約するGS
	void GS_1::ready() { gsm->reserve_transition(*this, new GS_2()); }
	void GS_1::update() { num += 5; }

	// サンプルゲームステート2
	void GS_2::ready() { num += 10; }
	void GS_2::update() { num += 8; }


	/************************************************************************************************************************************************/


	// 初回更新時に遷移を予約するオブジェクト
	void Obj1::ready() { gsm->reserve_transition(*this, new GS_2()); }
	bool Obj1::update() { num += 4; return true; }

	// 初回更新時に遷移を予約するオブジェクトを持つ
	void GS_3::ready() { add_object(new Obj1()); }
	void GS_3::update() { num += 3; }


	/************************************************************************************************************************************************/


	// サンプル1
	void Com1::ready() { num += 1; }
	void Com1::update() { num += 2; }

	// コンポーネント持ち
	void Obj2::ready() { add_component(new Com1()); }
	bool Obj2::update() { num += 3; return true; }

	// サンプル4
	void GS_4::ready() { add_object(new Obj2()); }
	void GS_4::update() { num += 4; }

	////////////////////////////////////////

	// サンプル1
	void Com1_1::ready() { num += 1; }
	void Com1_1::update() { num += 2; }

	// コンポーネント持ち
	void Obj2_1::ready() { add_component(std::make_shared<Com1_1>()); }
	bool Obj2_1::update() { num += 3; return true; }

	// サンプル4
	void GS_4_1::ready() { add_object(std::make_shared<Obj2_1>()); }
	void GS_4_1::update() { num += 4; }


	/************************************************************************************************************************************************/

	// 1
	void Com5_0::ready() {}
	void Com5_0::update() { num += 2; }

	// 3
	void Com5_1::ready() {}
	void Com5_1::update() { num *= 2; }

	// 2
	void Com5_2::ready() {}
	void Com5_2::update() { num += 10; }



	void Obj5::ready() {
		add_component(new Com5_0(3));
		add_component(new Com5_1(-3));
		add_component(new Com5_2(1));
	}
	bool Obj5::update() { num += 3; return true; }



	void GS_5::ready() { add_object(new Obj5()); }
	void GS_5::update() { num += 4; }

	/************************************************************************************************************************************************/

	// 2回目の更新時にコンポーネントを追加
	void Com6_0::ready() { num += 1; }
	void Com6_0::update() {
		num += 2;
		if (count == 1)
			object->add_component(new Com6_1());
		++count;
	}

	void Com6_1::ready() { num += 3; }
	void Com6_1::update() { num += 7; }



	// 2回目の更新時にコンポーネントを追加
	void Obj6::ready() {}
	bool Obj6::update() {
		num += 3;
		if (count == 1)
			add_component(new Com6_0());
		++count;
		return true;
	}



	// Obj6 を持つ
	void GS_6::ready() { add_object(new Obj6()); }
	void GS_6::update() {}

	/************************************************************************************************************************************************/

	void Com7::ready() { num += 2; }
	void Com7::update() { num += 3; }

	// コンポーネントの活性状態をいじる
	shared_ptr<Com7> Obj7_Com7;
	void Obj7::ready() { Obj7_Com7 = add_component(new Com7()); }
	bool Obj7::update() {
		num += 1;
		if (count % 2 == 0)
			Obj7_Com7->deactivate();
		else
			Obj7_Com7->activate();
		++count;
		return true;
	}

	// Obj7 を持つ
	void GS_7::ready() { add_object(new Obj7()); }
	void GS_7::update() {}

	/************************************************************************************************************************************************/

	void Obj8::ready() { num += 1; }
	bool Obj8::update() { num += 2; return true; }

	void GS_8::ready() { add_object(new Obj8()); }
	void GS_8::update() { num += 3; }

	/************************************************************************************************************************************************/

	void Obj9::ready() { num += get_priority(); }
	bool Obj9::update() { num += pos.x + pos.y + pos.z; return true; }

	void GS_9::ready() { add_object(new Obj9({ 1.0, 2.0, 3.0 }, 10)); }
	void GS_9::update() { num += 3; }

	/************************************************************************************************************************************************/
}
