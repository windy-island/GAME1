// Siv3D v0.6.16

// (catch2) 自分でmainを定義したい場合は下記のマクロ変数を定義
// 今回はmainを作られたくないので用意した
// あと、この定義はヘッダのincludeよりも先に書くこと
#define CATCH_CONFIG_RUNNER

#ifdef _DEBUG
#pragma comment(lib, "../SUPER_GAME_MAKER2/App/SUPER_GAME_MAKER2(debug).lib")
#else
#pragma comment(lib, "../SUPER_GAME_MAKER2/App/SUPER_GAME_MAKER2.lib")
#endif

#include "sgmPlus.h"
#include "./catch2.hpp"
#include <Windows.h>
#include <iostream>
#include <stdlib.h>

// コンソールの準備
void ready_console() {
	::AllocConsole();
	//SetConsoleCP(CP_UTF8);
	//SetConsoleOutputCP(CP_UTF8);
	//SetConsoleOutputCP(12000);
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);

	//SetConsoleOutputCP(12000);

	//s3d::Console.open();
}

// コマンドライン引数
class CommandLineArguments final {

	static constexpr int MAX_ARG_NUM = 30;
	static constexpr int MAX_ARG_SIZE = 100;

	int argc;
	char** argv;

public:
	CommandLineArguments() : argc(0), argv(nullptr) {

		// コマンドライン引数の取得
		const Array<String> commandsSrc = System::GetCommandLineArgs();
		argc = commandsSrc.size();
		//vector<char[MAX_ARG_SIZE]> commands(argc);
		//for (size_t i = 0; i < argc; ++i) {
		//	//strcpy_s(commands.at(i), sizeof(commands.at(i)), (char*)commandsSrc.at(i).narrow().c_str());
		//	//commands.at(i) = commandsSrc.at(i).narrow().c_str();
		//	const string str = commandsSrc.at(i).narrow();
		//	//std::char_traits<char>::copy(commands.at(i), str.c_str(), str.size() + 1);
		//	std::memcpy(commands.at(i), str.c_str(), MAX_ARG_SIZE);
		//	std::cout  << i << ": " << commandsSrc.at(i).narrow().c_str() << std::endl;
		//}

		//for (size_t i = 0; i < argc; ++i) {
		//	std::cout  << i << ": " << commands.at(i) << std::endl;
		//}

		// argvのセット
		argv = new char* [MAX_ARG_NUM];
		for (size_t i = 0; i < MAX_ARG_NUM; ++i) {
			argv[i] = new char[MAX_ARG_SIZE];
			argv[i][0] = '\0';
		}
		for (size_t i = 0; i < argc; ++i) {
			//argv[i] = (char*)commands.at(i);
			const string cmd = commandsSrc.at(i).narrow();
			std::memcpy(argv[i], cmd.c_str(), MAX_ARG_SIZE);
		}
	}

	~CommandLineArguments() {
		for (size_t i = 0; i < argc; ++i) {
			delete[] argv[i];
		}
		delete[] argv;
	}

	const int get_argument_count() const { return argc; }

	// ややこしいけど、参照先がconst
	// 2次元配列を指すポインタだけがconstじゃない
	char const* const* get_argument_values() const { return argv; }

	void log_argments() const {
		std::cout << "arg_count :" << argc << std::endl;
		for (size_t i = 0; i < argc; ++i) {
			std::cout << i << ": " << argv[i] << std::endl;
		}
	}
};

void Main() {

	ready_console();

	CommandLineArguments cmdArgs;

	// Catch2 による自動テスト
	const int ret = Catch::Session().run(cmdArgs.get_argument_count(), cmdArgs.get_argument_values());

	// 半自動テスト
	// 現在未実装
	string in = "";
	while (in != "y" && in != "n") {
		std::cout << "\n3Dカメラテスト1を実行しますか？( y or n )\n> ";
		std::cin >> in;
		std::this_thread::sleep_for(15ms);
	}

	::system("pause");
	return;
}
