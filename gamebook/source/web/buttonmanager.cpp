#include "buttonmanager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

string ButtonManager::GetLastPressed() {
	#ifdef __EMSCRIPTEN__
	return emscripten_run_script_string("lastPressed");
  	#endif
  return "none";
}

void ButtonManager::CreateButton(string button) {
	#ifdef __EMSCRIPTEN__
	EM_ASM({
		createButton(UTF8ToString($0));
	}, button.c_str());
	#endif
}

void ButtonManager::ResetButtons() {
    #ifdef __EMSCRIPTEN__
        EM_ASM({
            resetButtons();
        });
    #endif
}

int ButtonManager::GetButtonCount() {
	#ifdef __EMSCRIPTEN__
		return EM_ASM_INT({
			return document.getElementsByClassName("gameInput").lenght;
		});
	#endif
		return 0;
}
