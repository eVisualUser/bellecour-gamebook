#include "variablemanager.h"

#include <iostream>
#include <sstream>

#include "../debug/logger.h"
#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../render/console.h"

using namespace client_filesystem;

void VariableManager::CreateVariable(string name, int value) {
  Variable variable = {
      .name = name,
      .value = value,
  };

  this->AddVariable(variable);
}

void VariableManager::AddVariable(Variable variable) {
  this->buffer.push_back(variable);
}

int VariableManager::GetVariableValue(string name) {
  for (auto &i : this->buffer)
    if (i.name == name)
      return i.value;

  stringstream message;
  message << "Unknow variable: " << name;
  throw std::runtime_error(message.str());
}

bool VariableManager::IsExist(string name) {
  for (auto &var : this->buffer) {
    if (var.name == name)
      return true;
  }
  return false;
}

void VariableManager::SetVariableValue(string name, int value) {
  for (int i = 0; i < this->buffer.size(); i++) {
    if (this->buffer[i].name == name) {
      this->buffer[i].value = value;
      break;
    }
  }
  stringstream logMessage;
  logMessage << "Set " << name << " with " << value;
  Logger::Log(logMessage.str());
}

void VariableManager::Load(string path) {
  auto reader = Reader();
  try {
    reader.SetPath(path);
    reader.ReadFile();
  } catch (string message) {
    Logger::LogError(message);
    PrintError(message);
    exit(-1);
  }

  auto ini = Ini();
  ini.SetBuffer(reader.GetBuffer());
  auto table = ini.ParseTable("data");

  for (auto &var : table.GetAllVars()) {
    stringstream logMessage;
    logMessage << "Loaded Var: " << var.GetKey() << " = " << var.GetValue();
    Logger::Log(logMessage.str());

    Variable newVar;
    newVar.name = var.GetKey();
    newVar.value = TomlParseInt(var.GetValue());

    this->AddVariable(newVar);
  }
}
