#include <iostream>

#include "render/console.h"
#include "io/buttonmanager.h"

#include <string>
#include <vector>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

using namespace std;

long long int counter = 0;

auto buttonManager = ButtonManager();
auto console = Console();
vector<string> text;

void GameLoop() {
	buttonManager.Update();

	console.Clear();
	cout << counter << endl;

	if (counter < 10)
		cout << "LESS" << endl;
	else
		cout << "MORE" << endl;

	console.WaitAny();

	if (buttonManager.GetLastPressed() != "none") {
		buttonManager.ResetButtons();
		counter++;
		buttonManager.CreateButton("Increment");
	} else if (buttonManager.GetButtonCount() != 1)
		buttonManager.CreateButton("Increment");
}

int main() {
	console.SetWindow(600, 600);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(GameLoop, 0, 1);
#else
	while(true) {
		GameLoop();
	}
#endif
	return 0;
}
