#include <iostream>

#include "io/buttonmanager.h"
#include <string>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

using namespace std;

void game() {
	auto buttonManager = ButtonManager();
	if (buttonManager.GetButtonCount() < 5)
		buttonManager.CreateButton("myButton");
	cout << buttonManager.GetButtonCount() << endl;
	if (buttonManager.GetLastPressed() != "none") {
		cout << buttonManager.GetLastPressed() << endl;
	}
}

int main() {
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(game, 0, 1);
#endif
	return 0;
}
