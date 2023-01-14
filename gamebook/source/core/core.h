#pragma once

// File System
#include "../filesystem/reader.h"
#include "../filesystem/ini.h"

// Render
#include "../render/console.h"
#include "../render/ui.h"
#include "../render/rendermath.h"

// IO
#include "../io/buttonmanager.h"

// Logic Str
#include "../logicstr/executor.h"
#include "../logicstr/nodechain.h"
#include "../logicstr/variablemanager.h"
#include "../logicstr/actionmanager.h"
#include "../logicstr/conditionmanager.h"

// Page
#include "../pages/page.h"

// STD
#include <string>
#include <sstream>

using namespace std;

#define CORE_DEFAULT_CONFIG_FILE "config.ini"

class Core {
public:
	Core() { this->LoadConfig(CORE_DEFAULT_CONFIG_FILE); this->Initialize(); }

	void RunGameLoop();
	void LoadConfig(string path);
	
	void Initialize();

	void Update();
	void ClearScreen();
	void Draw();
	void UpdateInputs();
	void Render();
private:
	VariableManager _variableManager;
	ActionManager _actionManager;
	ConditionManager _conditionManager;

	Executor _executor;

	Page _page;
	ButtonManager _buttonManager;

	Console _console;
	UI _ui;

private:
	Point _frameSize;
	Point _textPosition;
	Point _buttonPosition;

	string _defaultPagePath; 
	string _defaultConfigPath;
};
