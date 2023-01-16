#include "inputmanager.h"
#include "iomath.h"
#include "../debug/logger.h"

#include <vector>
#include <iostream>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#else
	#include <conio.h> // Maybe not work with linux
#endif

string InputManager::GetLastPressed() {
	#ifdef __EMSCRIPTEN__
		return emscripten_run_script_string("lastPressed");
  	#else
  		return this->lastPressed;
  	#endif
}

void InputManager::CreateButton(string button) {
	#ifdef __EMSCRIPTEN__
		EM_ASM({
			createButton(UTF8ToString($0));
		}, button.c_str());
	#else
		this->buffer.push_back(button);
	#endif
}

void InputManager::ResetButtons() {
    #ifdef __EMSCRIPTEN__
    	emscripten_run_script("resetButtons();");
    #else
    	this->buffer = vector<string>();
		this->index = 0;
		this->lastPressed = "none";
	#endif
}

int InputManager::GetButtonCount() {
	#ifdef __EMSCRIPTEN__
		return emscripten_run_script_int("buttons.length");
	#else
		return this->buffer.size();
	#endif
}

void InputManager::Update() {
	#ifdef __EMSCRIPTEN__
	#else
		auto input = _getch();

		if (input==0 || input==0xE0) {
			input = _getch();

			if (input == KEY_UP) {
				this->index--;
			}
			else if (input == KEY_DOWN) {
				this->index++;
			}
		} else if (input == KEY_UNZOOM)
			this->unzoom = true;
		else if (input == KEY_ZOOM)
			this->zoom = true;
		else if (input == KEY_ENTER && !this->buffer.empty())
			this->lastPressed = this->buffer[this->index];
		else if (input == KEY_EXIT) {
			Logger::Log("Game Quit");
			exit(0);
		}

		clamp_int(&this->index, 0, this->GetButtonCount()-1);
	#endif
}

vector<string> InputManager::GetButtons() {
	#ifdef __EMSCRIPTEN__
	return vector<string>();
	#else
	return this->buffer;
	#endif
}

#ifdef __EMSCRIPTEN__
#else
int InputManager::GetIndex() {
	return this->index;
}
#endif

bool InputManager::Exists(string content) {
	#ifdef __EMSCRIPTEN__
		return false;
	#else
		bool result = false;

		for (auto & button: this->buffer) {
			if (content == button)
				result = false;
		}

		return result;
	#endif
}

bool InputManager::MustZoom() {
	bool result = this->zoom;
	this->zoom = false;
	return result;
}

bool InputManager::MustUnZoom() {
	bool result = this->unzoom;
	this->unzoom = false;
	return result;
}
