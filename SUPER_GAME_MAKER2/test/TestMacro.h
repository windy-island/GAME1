#pragma once

#include "SAMPLE_GameState.h"
#include <Windows.h>

/*
* テスト関数の書き方
*
* 戻り値はboolとする
* 最初にヘッダー、最後にフッダーのマクロを入れる
* あとは各マクロ関数を参照
*/

// GSMはシングルトンであるため2つ以上は生成できない
// テスト前に初期化しておく方がよい
#define TEST_ROOT_HEADER(className)																					\
gsm.reset();																										\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE);						\
std::cout << std::format("< {} >", className) << std::endl;															\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);



// 戻り値は変えた方よさそう
#define TEST_ROOT_FOOTER	\
gsm.reset();				\
std::cout << std::endl;		\
return true;



#define TEST_FUNC_HEADER	\
vector<string> errMessages;	\
int32 testCount = 0;



#define TEST_FUNC_FOOTER(funcName)																					\
if (!errMessages.empty()) {																							\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);										\
	std::cout << " [ ERROR ]";																						\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);	\
	std::cout << std::format(": {} ", funcName) << std::endl;														\
	std::cout << "ERR NUM: " << errMessages.size() << std::endl;													\
	for (const auto& mes : errMessages) {																			\
		std::cout << mes << std::endl;																				\
	}																												\
	std::cout << std::endl;																							\
	return false;																									\
}																													\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);											\
std::cout << " [  OK  ]";																							\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);		\
std::cout << std::format(": {}", funcName) << std::endl;															\
return true;


#define SHOULD_NOT_THROW_EXCEPTION(code, errorMessage)												\
do {																								\
	gsm.reset();																					\
	num = 0;																						\
	try {																							\
		code;																						\
	}																								\
	catch (const std::exception& e) {																\
		errMessages.push_back(std::format("[ {} ]: ({}) {}", testCount, e.what(), errorMessage));	\
	}																								\
	++testCount;																					\
	gsm.reset();																					\
	num = 0;																						\
}																									\
while (0);


// 例外を出すのが正しい場合に使用
// 第一引数: 例外を出すべきコード
// 第二引数: エラーメッセージ
#define SHOULD_THROW_EXCEPTION(code, errorMessage)									\
do {																				\
	gsm.reset();																	\
	num = 0;																		\
	try {																			\
		code;																		\
		errMessages.push_back(std::format("[ {} ]: {}", testCount, errorMessage));	\
	}																				\
	catch ([[maybe_unused]] const std::exception& e) {								\
	}																				\
	++testCount;																	\
	gsm.reset();																	\
	num = 0;																		\
}																					\
while (0);



#define SHOULD_TRUE(process, condition, errorMessageFalse, errorMessageException)							\
do {																										\
	gsm.reset();																							\
	num = 0;																								\
	try {																									\
		process;																							\
		if (!condition) {																					\
			errMessages.push_back(std::format("[ {} ]: {}", testCount, errorMessageFalse));					\
		}																									\
	}																										\
	catch (const std::exception& e) {																		\
		errMessages.push_back(std::format("[ {} ]: ({}) {}", testCount, e.what(), errorMessageException));	\
	}																										\
	++testCount;																							\
	gsm.reset();																							\
	num = 0;																								\
}																											\
while (0);
