#pragma once

#include <string>
#include <vector>

#include "../logicstr/conditionmanager.h"
#include "../logicstr/variablemanager.h"
#include "iomath.h"

using namespace std;

/// Work only on the web
class InputManager {
public:
  void Update();
  int GetButtonCount();
  void CreateButton(string button);
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
