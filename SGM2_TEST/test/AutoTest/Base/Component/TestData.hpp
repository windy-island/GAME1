#pragma once

#include "sgmPlus.h"
#include "MacroSet.h"

namespace SGM2_TEST::Component {

	int32 num = 0;



	/********************************************************************************************************************************************************************/

	//COM_SIMPLE(
	//	A, {}, {
	//		num += 1;
	//	}, {
	//		num += 2;
	//	}
	//);

	COM_SIMPLE(A);
	inline void ComA::ready() { num += 1; }
	inline void ComA::update() { num += 2; }

	//OBJ_SIMPLE(
	//	A, {}, {
	//		add_component(new ComA());
	//	}, {
	//		num += 3; return true;
	//	}
	//);

	OBJ_SIMPLE(A);
	inline void ObjA::ready() { add_component(new ComA()); }
	inline bool ObjA::update() { num += 3; return true; }

	//GS_SIMPLE(
	//	A, {}, {
	//		add_object(new ObjA());
	//	}, {
	//		num += 4;
	//	}
	//);

	GS_SIMPLE(A);
	inline void GS_A::ready() { add_object(new ObjA()); }
	inline void GS_A::update() { num += 4; }

	///////////////////////////////////////////////////////////////////////

	OBJ_SIMPLE(A1);
	inline void ObjA1::ready() { add_component(std::make_shared<ComA>()); }
	inline bool ObjA1::update() { num += 3; return true; }



	GS_SIMPLE(A1);
	inline void GS_A1::ready() { add_object(std::make_shared<ObjA1>()); }
	inline void GS_A1::update() { num += 4; }

	/********************************************************************************************************************************************************************/

	// 1
	COM_SIMPLE(B);
	inline void ComB::ready() {};
	inline void ComB::update() { num += 2; }

	// 3
	COM_SIMPLE(B1);
	inline void ComB1::ready() {};
	inline void ComB1::update() { num *= 2; }

	// 2
	COM_SIMPLE(B2);
	inline void ComB2::ready() {};
	inline void ComB2::update() { num += 10; }



	OBJ_SIMPLE(B);
	inline void ObjB::ready() {
		add_component(new ComB(3));
		add_component(new ComB1(-3));
		add_component(new ComB2(1));
	}
	inline bool ObjB::update() {
		num += 3;
		return true;
	}



	GS_SIMPLE(B);
	inline void GS_B::ready() { add_object(new ObjB()); }
	inline void GS_B::update() { num += 4; }

	/********************************************************************************************************************************************************************/

	COM_SIMPLE(C0);
	inline void ComC0::ready() { num += 3; }
	inline void ComC0::update() { num += 7; }

	// 2度目の更新時にコンポーネントを追加
	COM_COUNTER(C1);
	inline void ComC1::ready() { num += 1; }
	inline void ComC1::update() {
		num += 2;
		if (count == 1)
			object->add_component(new ComC0());
		++count;
	}



	// 2度目の更新時にコンポーネントを追加
	OBJ_COUNTER(C);
	void ObjC::ready() {}
	bool ObjC::update() {
		num += 3;
		if (count == 1) {
			add_component(new ComC1());
		}
		++count;
		return true;
	}


	
	// Obj6を持つ
	GS_SIMPLE(C);
	inline void GS_C::ready() { add_object(new ObjC()); }
	inline void GS_C::update() {}

	/********************************************************************************************************************************************************************/

	COM_SIMPLE(D);
	inline void ComD::ready() { num += 2; }
	inline void ComD::update() { num += 3; }



	// オブジェクトの活性状態をいじる
	shared_ptr<ComD> ObjD_pComD;
	OBJ_COUNTER(D);
	inline void ObjD::ready() { ObjD_pComD = add_component(new ComD()); }
	inline bool ObjD::update() {
		num += 1;
		if (count % 2 == 0) {
			ObjD_pComD->deactivate();
		}
		else {
			ObjD_pComD->activate();
		}
		++count;
		return true;
	}



	// ObjDを持つ
	GS_SIMPLE(D);
	inline void GS_D::ready() { add_object(new ObjD()); }
	inline void GS_D::update() {}

	/********************************************************************************************************************************************************************/
}




