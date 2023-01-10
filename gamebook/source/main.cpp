#include <iostream>

#include "filesystem/ini.h"
#include "filesystem/reader.h"
#include "filesystem/toml.h"
#include "logicstr/executor.h"
#include "logicstr/nodechain.h"
#include "logicstr/variablemanager.h"

#include <string.h>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

using namespace std;
int main() {
	cout << "Here will be playable the game-book !" << endl;

	auto reader = Reader();
	reader.SetPath("assets/demo.toml");
	reader.ReadFile();

	auto nodeChain = NodeChain();
	nodeChain.ParseString("10 < 20"); // Must return true

	auto variableManager = VariableManager();

	auto executor = Executor();
	cout << "\033[31mResult: " << (executor.ExecuteConditionComand(&variableManager, &nodeChain) == true ? "true" : "false") << endl;

#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif

	return 0;
}
