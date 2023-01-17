#pragma once

// File System
#include "../filesystem/ini.h"
#include "../filesystem/reader.h"

// Render
#include "../render/console.h"
#include "../render/rendermath.h"
#include "../render/ui.h"

// IO
#include "../io/inputmanager.h"

// Logic Str
#include "../logicstr/actionmanager.h"
#include "../logicstr/conditionmanager.h"
#include "../logicstr/executor.h"
#include "../logicstr/nodechain.h"
#include "../logicstr/variablemanager.h"

// Page
#include "../pages/page.h"

// STD
#include <sstream>
#include <string>

#define DEFAULT_CONFIG_PATH "config.toml"

using namespace std;

class Core {
public:
  Core() {
    this->LoadConfig();
    this->Initialize();
  }

  void RunGameLoop();
  void LoadConfig();

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
  InputManager _inputManager;

  Console _console;
  UI _ui;

private:
  Point _frameSize;

  string _defaultPagePath;
};