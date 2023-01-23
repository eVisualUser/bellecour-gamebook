#include "inputmanager.h"
#include "../debug/logger.h"
#include "../render/console.h"
#include "iomath.h"
#include "../filesystem/reader.h"
#include "../filesystem/ini.h"
#include "../logicstr/stringformater.h"

#include <iostream>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <conio.h> // Maybe not work with linux
#endif

string InputManager::GetLastPressed() {
#ifdef __EMSCRIPTEN__
  return emscripten_run_script_string("lastPressed");
#else
  return this->lastPressed;
#endif
}

void InputManager::CreateButton(string button, VariableManager *variableManager, Executor *executor) {
#ifdef __EMSCRIPTEN__
  button = ReplaceVariables(button, variableManager, executor);
  EM_ASM({ createButton(UTF8ToString($0)); }, button.c_str());
#else
  this->buffer.push_back(button);
#endif
}

void InputManager::ResetButtons() {
#ifdef __EMSCRIPTEN__
  emscripten_run_script("resetButtons();");
#else
  this->buffer = vector<string>();
  this->index = 0;
  this->lastPressed = "none";
#endif
}

int InputManager::GetButtonCount() {
#ifdef __EMSCRIPTEN__
  return emscripten_run_script_int("buttons.length");
#else
  return this->buffer.size();
#endif
}

bool InputManager::Update() {
#ifdef __EMSCRIPTEN__
#else
  auto input = _getch();

    if (input == 0 || input == 0xE0) {
      input = _getch();
      if (input == this->_keyUp) {
        this->index--;
      } else if (input == this->_keyDown) {
        this->index++;
      } else if (input == this->_keyRefresh) {
        return true;
      } else {
        this->Update();
      }
    } else if (input == this->_keyUnZoom)
      this->unzoom = true;
    else if (input == this->_keyZoom)
      this->zoom = true;
    else if (input == this->_keyOk && !this->buffer.empty())
      this->lastPressed = this->buffer[this->index];
    else if (input == this->_keyExit) {
      Logger::Log("Game Quit");
      exit(0);
    } else {
        this->Update();
    }

  clamp_int(&this->index, 0, this->GetButtonCount() - 1);
#endif
  return false;
}

vector<string> InputManager::GetButtons() {
#ifdef __EMSCRIPTEN__
  return vector<string>();
#else
  return this->buffer;
#endif
}

#ifdef __EMSCRIPTEN__
#else
int InputManager::GetIndex() { return this->index; }
#endif

bool InputManager::Exists(string content) {
#ifdef __EMSCRIPTEN__
  return false;
#else
  bool result = false;

  for (auto &button : this->buffer) {
    if (content == button)
      result = false;
  }

  return result;
#endif
}

bool InputManager::MustZoom() {
#ifdef __EMSCRIPTEN__
  return false;
#else
  bool result = this->zoom;
  this->zoom = false;
  return result;
#endif
}

bool InputManager::MustUnZoom() {
#ifdef __EMSCRIPTEN__
  return false;
#else
  bool result = this->unzoom;
  this->unzoom = false;
  return result;
#endif
}

void InputManager::Remove(string button) {
#ifdef __EMSCRIPTEN__
#else
    for (int i = 0; i < this->buffer.size()-1; i++) {
      if (this->buffer[i] == button) {
        this->buffer.erase(this->buffer.cbegin() + i);
      }
    }
#endif
}
