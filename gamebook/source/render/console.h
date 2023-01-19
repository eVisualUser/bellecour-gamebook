#pragma once

#include <string>
#include <vector>
#include <iostream>

#define COLOR_FOREGROUND_GREEN 32
#define COLOR_BACKGROUND_BLACK 40
#define COLOR_DEFAULT 0

class Console {
public:
	Console();
	~Console();
	void SetWindow();
	void WaitAny();
	void Clear();
	void PrintFrame(std::vector<std::string> *frame);
    void SetConsoleColor(int colorForeground);
    int ConvertColor(int ansiColor);
};

void PrintError(std::string error);
