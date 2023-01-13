#include <iostream>

#include "render/console.h"
#include "io/buttonmanager.h"
#include "render/ui.h"
#include "render/rendermath.h"
#include "logicstr/executor.h"
#include "logicstr/nodechain.h"
#include "logicstr/variablemanager.h"
#include "pages/actionmanager.h"

#include "pages/page.h"

#include <iterator>
#include <string>
#include <vector>
#include <sstream> 

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

using namespace std;

auto buttonManager = ButtonManager();
auto console = Console();
auto ui = UI(Point(50, 25));

auto variableManager = VariableManager();
auto actionManager = ActionManager();
auto executor = Executor();
Page page;

vector<string> text;
string nextPage = "none";

bool toogle = false;

void GameLoop() {
	try {
		console.Clear();

		ui.ResetBuffer();

		ui.DrawButtons(Point(15, 10), &buttonManager);

		ui.DrawText(Point(5, 0), page.name);

		for (auto & textLine: page.textContent)
			ui.DrawText(Point(0, 5), textLine);

		auto frame = ui.GetFrame();
		console.PrintFrame(&frame);

		buttonManager.Update();

		nextPage = page.GetButtonPressed(
			buttonManager.GetLastPressed(),
			 &executor,
			  &actionManager,
			   &variableManager);
		
		if (!nextPage.empty()) {
			page = Page();
			stringstream stream;
			stream << "pages/" << nextPage;
			page.Load(stream.str());
			buttonManager.ResetButtons();
			page.CreateButtons(&buttonManager);
		}
	} catch(const string message) {
		cout << message << endl;
	}
}

int main() {
	console.SetWindow(600, 600);
	variableManager.Load("config.toml");
	actionManager.Load("config.toml");
	
	page.Load("pages/index.toml");
	page.CreateButtons(&buttonManager);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(GameLoop, 0, 1);
#else
	while(true) {
		GameLoop();
	}
#endif
	return 0;
}
