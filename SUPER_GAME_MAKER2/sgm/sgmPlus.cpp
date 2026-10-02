#include "sgmPlus.h"

// 乱数生成器

//std::random_device seedGenerator;
//std::mt19937_64 random(seedGenerator()); <- なぜかデバッグ時のみエラー。別なのを使うことにする。

std::mt19937_64 random((uint32)std::chrono::high_resolution_clock::now().time_since_epoch().count());

// ゲームステートマネージャー
unique_ptr<SGM2::GameStateManager> gsm;
