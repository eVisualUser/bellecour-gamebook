#pragma once

#include <string>
#include <vector>
#include <iostream>

#define COLOR_FOREGROUND_GREEN "\033[32m"
#define COLOR_BACKGROUND_BLACK "\033[40m"
#define COLOR_DEFAULT "\033[0m"

class Console {
public:
	Console();
	~Console();
	void SetWindow(int x, int y);
	void WaitAny();
	void Clear();
	void PrintFrame(std::vector<std::string> *frame);
};
