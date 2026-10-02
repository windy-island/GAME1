#pragma once

#include "sgmPlus.h"
#include "MacroSet.h"

namespace SGM2_TEST::GameStateManager {

	int32 num = 0;



	/********************************************************************************************************************************************************************/

	GS_SIMPLE(A);
	inline void GS_A::ready() {}
	inline void GS_A::update() {}



	/********************************************************************************************************************************************************************/

	GS_SIMPLE(B1);
	GS_SIMPLE(B2);

	inline void GS_B1::ready() { gsm->reserve_transition(*this, new GS_B2()); }
	inline void GS_B1::update() { num += 5; }

	inline void GS_B2::ready() { num += 10; }
	inline void GS_B2::update() { num += 8; }



	/********************************************************************************************************************************************************************/

	OBJ_SIMPLE(C);
	GS_SIMPLE(C1);
	GS_SIMPLE(C2);

	inline void ObjC::ready() { gsm->reserve_transition(*this, new GS_C2()); }
	inline bool ObjC::update() { num += 4; return true; }
	
	inline void GS_C1::ready() { add_object(new ObjC()); }
	inline void GS_C1::update() { num += 3; }

	inline void GS_C2::ready() { num += 10; }
	inline void GS_C2::update() { num += 8; }
}
