#include <iostream>

#include "render/console.h"
#include "io/buttonmanager.h"

#include <string>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

using namespace std;

long long int counter = 0;

auto buttonManager = ButtonManager();
auto console = Console();

void GameLoop() {
	buttonManager.Update();

	if (buttonManager.GetLastPressed() != "none") {
		cout << "You: " << buttonManager.GetLastPressed() << endl;
		buttonManager.ResetButtons();
		buttonManager.CreateButton("Hello !");
	} else if (buttonManager.GetButtonCount() != 1)
		buttonManager.CreateButton("Hello !");
}

int main() {
	console.SetWindow(600, 600);
	cout << "Game: Hello !" << endl;

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(GameLoop, 0, 1);
#else
	while(true) {
		GameLoop();
	}
#endif
	return 0;
}
