#pragma once

#include <vector>
#include <string>

#include "rendermath.h"
#include "../io/buttonmanager.h"

using namespace std;

class UI {
public:
	UI(Point newSize);
public:
	void DrawRawLine(Point position, int lenght, char character);
	void DrawRawRect(Rect rect, char character);
	void DrawText(Point start, string text);
public:
	void InitializeBuffer();
	void ResetBuffer();
	vector<string> GetFrame();
	void DrawButtons(Point position, ButtonManager *buttonManager);
private:
	vector<string> buffer;
	Point size;
};
