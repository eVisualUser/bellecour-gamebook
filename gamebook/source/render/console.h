#pragma once

#include <string>

using namespace std;

class Console {
public:
	void PrintText(string input);
	void PrintLine();
	void MoveCursor();
	void Clear();
private:
	float printSpeed = 1;
	bool proceduralTextPrint = false;
};
