#include "actionmanager.h"

#include "../debug/logger.h"
#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../render/console.h"

#include <iostream>
#include <sstream>
#include <stdexcept>


void ActionManager::Load(string path) {
  auto reader = Reader();
  try {
    reader.SetPath(path);
    reader.ReadFile();
  } catch (runtime_error message) {
    Logger::LogError(message.what());
    PrintError(message.what());
    exit(-1);
  }

  auto ini = Ini();
  ini.SetBuffer(reader.GetBuffer());
  auto table = ini.ParseTable("actions");

  for (auto &var : table.GetAllVars()) {
    Action newAction;
    newAction.list = TomlParseArray(var.GetValue());
    for (auto &action : newAction.list) {
      action = TomlParseString(action);
    }
    newAction.name = var.GetKey();
    this->actions.push_back(newAction);
  }
}

Action ActionManager::GetAction(string name) {
  for (auto &action : this->actions) {
    if (action.name == name)
      return action;
  }
  stringstream message;
  message << "Action missing: " << name;
  throw runtime_error(message.str());
}
