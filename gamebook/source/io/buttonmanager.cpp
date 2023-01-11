#include "buttonmanager.h"
#include "iomath.h"

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#else
	#include <conio.h>
#endif

string ButtonManager::GetLastPressed() {
	#ifdef __EMSCRIPTEN__
		return emscripten_run_script_string("lastPressed");
  	#else
  		return this->lastPressed;
  	#endif
}

void ButtonManager::CreateButton(string button) {
	#ifdef __EMSCRIPTEN__
		EM_ASM({
			createButton(UTF8ToString($0));
		}, button.c_str());
	#else
		this->buffer.push_back(button);
	#endif
}

void ButtonManager::ResetButtons() {
    #ifdef __EMSCRIPTEN__
    	emscripten_run_script("resetButtons();");
    #else
    	this->buffer = vector<string>();
		this->index = 0;
		this->lastPressed = "none";
	#endif
}

int ButtonManager::GetButtonCount() {
	#ifdef __EMSCRIPTEN__
		return emscripten_run_script_int("buttons.length");
	#else
		return this->buffer.size() - 1;
	#endif
}

void ButtonManager::Update() {
	#ifdef __EMSCRIPTEN__
	#else
		auto input = _getch();

		if (input==0 || input==0xE0) {
			input = _getch();

			if (input == KEY_UP)
				this->index++;
			else if (input == KEY_DOWN)
				this->index--;

		} else if (input == KEY_ENTER && !this->buffer.empty()) {
			this->lastPressed = this->buffer[this->index];
		}
		clamp_int(&this->index, 0, this->GetButtonCount());
	#endif
}
