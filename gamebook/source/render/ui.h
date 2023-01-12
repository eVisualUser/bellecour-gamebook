#pragma once

#include <vector>
#include <string>

#include "rendermath.h"

using namespace std;

class UI {
public:
	UI(Point newSize);
public:
	void DrawRawLine(Point position, int lenght, char character);
	void DrawRawRect(Rect rect, char character);
public:
	void InitializeBuffer();
	void ResetBuffer();
	vector<string> GenerateFrame();
private:
	vector<string> buffer;
	Point size;
};
