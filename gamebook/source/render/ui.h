#pragma once

#include <vector>
#include <string>

#include "rendermath.h"
#include "../io/inputmanager.h"
#include "../pages/page.h"
#include "../logicstr/stringformater.h"
#include "../logicstr/variablemanager.h"

using namespace std;

class UI {
public:
	UI() {}
	UI(Point newSize);
public:
	void DrawRawLine(Point position, int lenght, char character);
	int DrawText(Point start, string text, VariableManager* variableManager);
	void Zoom();
	void UnZoom();
public:
	void InitializeBuffer();
	void ResetBuffer();
	vector<string> GetFrame();
        void DrawButtons(Point position, InputManager *buttonManager, char selectedChar, VariableManager* variableManager);
	Point size;
private:
	vector<string> buffer;
};
