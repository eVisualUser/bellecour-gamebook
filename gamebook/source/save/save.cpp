#include "save.h"

#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../render/console.h"
#include "../debug/logger.h"
#include "../logicstr/variablemanager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace client_filesystem;

void LoadSave(VariableManager* variableManager, Page *page, string path, string save) {
    auto reader = Reader();
    try {
        reader.SetPath(path);
        reader.ReadFile();
    } catch (runtime_error error) {
        std::ofstream file(path,std::ios::app);
        file.close();
        LoadSave(variableManager, page, path, save);
    }

    stringstream message;
    message << "Load save: " << save << " From " << path;
    Logger::Log(message.str());

    auto ini = Ini();
    ini.SetBuffer(reader.GetBuffer());

    auto saveTable = ini.ParseTable(save);

    *variableManager = VariableManager();

    for (auto & var: saveTable.GetAllVars()) {
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

void Save(VariableManager* variableManager, Page *page, string path) {
    string saveName = Logger::GetStrTime();

    fstream file;
    file.open(path, ios::out | ios::app);

    if (!file) {
      stringstream message;
      message << "Failed to open/create" << path;
      Logger::LogError(message.str());
      PrintError(message.str());
      exit(-1);
    }

    file << '[' << saveName << ']' << endl;
    file << "SAVE_PAGE" << " = " << '\"' << page->fileName << '\"' << endl;

    for (auto & var: variableManager->GetAllVariables()) {
        file << var.name << " = " << var.value << endl;
    }

    file.close();
}
