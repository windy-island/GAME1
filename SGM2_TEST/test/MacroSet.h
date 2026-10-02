#pragma once

#include "sgmPlus.h"

// ### メモ ### //
//
// TestData を複数個所から include しても問題ないよう、関数定義は inline 化しといた方がいい。
// 多重定義エラーを避けるため。

// ■ // ■ // ■ // ■ // ■ // ■ // ■ // COMPONENT // ■ // ■ // ■ // ■ // ■ // ■ // ■ //

//#define COM_COUNTER(prefix_, constructor_, ready_, update_)		\
//class Com##prefix_ : public Component {							\
//public:															\
//	constexpr Com##prefix_(const int priority_ = 0) :			\
//		Component(priority_), count(0) { constructor_; }		\
//private:														\
//	int count;													\
//	void ready() override { ready_; }							\
//	void update() override { update_; }							\
//};

#define COM_SIMPLE(prefix_)														\
class Com##prefix_ : public Component {											\
public:																			\
	constexpr Com##prefix_(const int priority_ = 0) : Component(priority_) {}	\
private:																		\
	void ready() override;														\
	void update() override;														\
};

#define COM_COUNTER(prefix_)																\
class Com##prefix_ : public Component {														\
public:																						\
	constexpr Com##prefix_(const int priority_ = 0) : Component(priority_), count(0) {}		\
private:																					\
	int count;																				\
	void ready() override;																	\
	void update() override;																	\
};





// ■ // ■ // ■ // ■ // ■ // ■ // ■ // OBJECT // ■ // ■ // ■ // ■ // ■ // ■ // ■ //

//#define OBJ_SIMPLE(prefix_, constructor_, ready_, update_)							\
//class Obj##prefix_: public Object {													\
//public:																				\
//	Obj##prefix_(const Vec3& pos = { 0, 0, 0 }, const int priority_ = 0) :			\
//		Object(pos, priority_) { constructor_; }									\
//private:																			\
//	void ready() override { ready_; }												\
//	bool update() override { update_; }												\
//};

#define OBJ_SIMPLE(prefix_)															\
class Obj##prefix_: public Object {													\
public:																				\
	Obj##prefix_(const Vec3& pos = { 0, 0, 0 }, const int priority_ = 0) :			\
		Object(pos, priority_) {}													\
private:																			\
	void ready() override;															\
	bool update() override;															\
};

#define OBJ_COUNTER(prefix_)														\
class Obj##prefix_: public Object {													\
public:																				\
	Obj##prefix_(const Vec3& pos = { 0, 0, 0 }, const int priority_ = 0) :			\
		Object(pos, priority_), count(0) {}											\
private:																			\
	int count;																		\
	void ready() override;															\
	bool update() override;															\
};





// ■ // ■ // ■ // ■ // ■ // ■ // ■ // GAMESTATE // ■ // ■ // ■ // ■ // ■ // ■ // ■ //

//#define GS_SIMPLE(prefix_, constructor_, ready_, update_)			\
//class GS_##prefix_: public GameState {								\
//public:																\
//	GS_##prefix_() { constructor_; }								\
//private:															\
//	void ready() override { ready_; }								\
//	void update() override { update_; }								\
//};

#define GS_SIMPLE(prefix_)											\
class GS_##prefix_: public GameState {								\
public:																\
	GS_##prefix_() {}												\
private:															\
	void ready() override;											\
	void update() override;											\
};

#define GS_COUNTER(prefix_)											\
class GS_##prefix_: public GameState {								\
public:																\
	GS_##prefix_() : count(0) {}									\
private:															\
	int count;														\
	void ready() override;											\
	void update() override;											\
};
