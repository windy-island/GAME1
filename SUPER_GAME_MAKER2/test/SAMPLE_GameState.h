#pragma once

#include "sgmPlus.h"

#define S_COM(No)									\
class Com##No : public Component {					\
public:												\
	constexpr Com##No(const int priority_ = 0) :	\
		Component(priority_), count(0) {}			\
private:											\
	int count;										\
	void ready() override;							\
	void update() override;							\
};

#define S_OBJ(No)														\
class Obj##No : public Object {											\
public:																	\
	Obj##No(const Vec3& pos = { 0, 0, 0 }, const int priority_ = 0) :	\
		Object(pos, priority_), count(0) {}								\
private:																\
	int count;															\
	void ready() override;												\
	bool update() override;												\
};

#define S_GS(No)					\
class GS_##No : public GameState {	\
public:								\
	GS_##No() : count(0) {}			\
private:							\
	int count;						\
	void ready() override;			\
	void update() override;			\
};

namespace SGM2_TEST {

	extern int32 num;

	S_GS(1);
	S_GS(2);

	S_OBJ(1);
	S_GS(3);

	S_COM(1);
	S_OBJ(2);
	S_GS(4);
	S_COM(1_1);
	S_OBJ(2_1);
	S_GS(4_1);

	S_COM(5_0);
	S_COM(5_1);
	S_COM(5_2);
	S_OBJ(5);
	S_GS(5);

	S_COM(6_0);
	S_COM(6_1);
	S_OBJ(6);
	S_GS(6);

	S_COM(7)
	S_OBJ(7)
	S_GS(7)

	S_OBJ(8);
	S_GS(8);

	S_OBJ(9);
	S_GS(9);

	//●// ゲームステート //●//

	class GS_A : public GameState {
	public:
		GS_A() : font(60) {}
		int32 get_8() { return 8; }

	private:
		Font font;
		void ready() {}
		void update() override {}
		void draw() const override {
			font(U"GS_A").drawAt(Scene::Center());
		}
	};
}
