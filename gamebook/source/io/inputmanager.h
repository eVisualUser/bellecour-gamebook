#pragma once

#include <string>
#include <vector>

#include "../logicstr/conditionmanager.h"
#include "../logicstr/executor.h"
#include "../logicstr/variablemanager.h"
#include "iomath.h"

using namespace std;

/// Work only on the web
class InputManager {
public:
  /// Return if it need to refrash the current page
  bool Update();
  int GetButtonCount();
  void CreateButton(string button, VariableManager *variableManager,
                    Executor *executor);
  string GetLastPressed();
  void ResetButtons();
  vector<string> GetButtons();
  bool Exists(string button);
  bool MustZoom();
  bool MustUnZoom();
  void Remove(string button);

public:
  int _keyOk = 13;
  int _keyUp = 72;
  int _keyDown = 80;
  int _keyExit = 27;
  int _keyZoom = 61;
  int _keyUnZoom = 45;
  int _keyRefresh = 63;
#ifdef __EMSCRIPTEN__
#else
  int GetIndex();

private:
  string lastPressed = "none";
  int index = 0;
  vector<string> buffer;
  bool zoom = false;
  bool unzoom = false;
#endif
};
