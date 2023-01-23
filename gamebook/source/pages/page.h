#pragma once

#include <string>
#include <vector>

#include "button.h"

#include "../io/inputmanager.h"
#include "../logicstr/variablemanager.h"
#include "../logicstr/executor.h"
#include "../logicstr/actionmanager.h"
#include "../logicstr/conditionmanager.h"

using namespace std;

class Page {
public:
	void Load(string path);
    void CreateButtons(InputManager *inputManager, VariableManager *variableManager, Executor *executor);
	string GetButtonPressed(string content, Executor *executor, ActionManager *actionManager, VariableManager *variableManager, ConditionManager *conditionManager);
	bool IsButtonActive(string content, Executor *executor, VariableManager *variableManager, ConditionManager *conditionManager);
    vector<Button> GetButtons();

public:
	string name;
	string type;
	vector<string> textContent;
	vector<Button> buttons;
    string fileName;
};
