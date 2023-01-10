#pragma once

#include <vector>
#include <string>

using namespace std;

/// Work only on the web
class ButtonManager {
public:
	void Update();
	int GetButtonCount();
	void CreateButton(string button);
	string GetLastPressed();
	void ResetButtons();
};
