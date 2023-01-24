#include "save.h"

#include "../debug/logger.h"
#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../logicstr/variablemanager.h"
#include "../render/console.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

void LoadSave(VariableManager *variableManager, Page *page, string filePath,
              string save) {
  auto reader = Reader();
  try {
    reader.SetPath(filePath);
    reader.ReadFile();
  } catch (runtime_error error) {
    exit(-1);
  }

  stringstream message;
  message << "Load save: " << save << " From " << filePath;
  Logger::Log(message.str());

  auto ini = Ini();
  ini.SetBuffer(reader.GetBuffer());

  auto saveTable = ini.ParseTable(save);

  *variableManager = VariableManager();

  for (auto &var : saveTable.GetAllVars()) {
    Variable newVar;
    if (var.GetKey() != "SAVE_PAGE") {
      newVar.name = var.GetKey();
      newVar.value = TomlParseInt(var.GetValue());

      variableManager->AddVariable(newVar);
    }
  }
  auto var = saveTable.GetVar("SAVE_PAGE");

  *page = Page();
  page->Load(TomlParseString(var.GetValue()));
}

void Save(VariableManager *variableManager, Page *page, string filePath) {
  string saveName = Logger::GetStrTime();

  fstream file;
  file.open(filePath, ios::out | ios::app);

  if (!file) {
    stringstream message;
    message << "Failed to open/create" << filePath;
    Logger::LogError(message.str());
    PrintError(message.str());
    exit(-1);
  }

  auto reader = Reader();
  try {
    reader.SetPath(filePath);
    reader.ReadFile();
  } catch (runtime_error error) {
    Logger::LogError(error.what());
    PrintError(error.what());
  }

  auto ini = Ini();
  ini.SetBuffer(reader.GetBuffer());

  bool saveExist = false;
  for (auto &tableName : ini.GetAllTables()) {
    if (tableName == saveName)
      saveExist = true;
  }

  if (!saveExist) {
    file << '[' << saveName << ']' << endl;
    file << "SAVE_PAGE"
         << " = " << '\"' << page->fileName << '\"' << endl;

    for (auto &var : variableManager->GetAllVariables()) {
      file << var.name << " = " << var.value << endl;
    }
  }

  file.close();
}
