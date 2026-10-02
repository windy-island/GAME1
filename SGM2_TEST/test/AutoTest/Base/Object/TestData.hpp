#pragma once
#include "sgmPlus.h"
#include "MacroSet.h"

namespace SGM2_TEST::Object {

	int32 num = 0;



	/********************************************************************************************************************************************************************/

	OBJ_SIMPLE(A);
	void ObjA::ready() { num += 1; }
	bool ObjA::update() { num += 2; return true; }

	GS_SIMPLE(A);
	void GS_A::ready() { add_object(new ObjA()); }
	void GS_A::update() { num += 3; }



	/********************************************************************************************************************************************************************/

	OBJ_SIMPLE(B);
	void ObjB::ready() { num += get_priority(); }
	bool ObjB::update() { num += pos.x + pos.y + pos.z; return true; }

	GS_SIMPLE(B);
	void GS_B::ready() { add_object(new ObjB({ 1.0, 2.0, 3.0}, 10)); }
	void GS_B::update() { num += 3; }
}
