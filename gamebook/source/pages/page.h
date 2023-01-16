#pragma once

#include <string>
#include <vector>

#include "button.h"

#include "../io/inputmanager.h"
#include "../logicstr/variablemanager.h"
#include "../logicstr/executor.h"
#include "../logicstr/actionmanager.h"

using namespace std;

class Page {
public:
	void Load(string path);
	void CreateButtons(InputManager *inputManager);
	string GetButtonPressed(string content, Executor *executor, ActionManager *actionManager, VariableManager *variableManager);
public:
	string name;
	string type;
	vector<string> textContent;
	vector<Button> buttons;
};
