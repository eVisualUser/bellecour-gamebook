#pragma once

#include <vector>
#include <string>

#include "iomath.h"

using namespace std;

#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80

#define KEY_EXIT 27

#define KEY_ZOOM 61
#define KEY_UNZOOM 45

/// Work only on the web
class InputManager {
public:
	void Update();
	int GetButtonCount();
	void CreateButton(string button);
	string GetLastPressed();
	void ResetButtons();
	vector<string> GetButtons(); 
	bool Exists(string button);
	bool MustZoom();
	bool MustUnZoom();
#ifdef __EMSCRIPTEN__
#else
	int GetIndex();
private:
	string lastPressed = "none";
	int index = 0;
	vector<string> buffer;
	bool zoom = false;
	bool unzoom = false;
#endif
};
