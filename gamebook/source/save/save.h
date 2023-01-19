#pragma once

#include "../pages/page.h"
#include "../logicstr/variablemanager.h"

#include <string>

using namespace std;

void LoadSave(VariableManager* variableManager, Page *page, string path, string save);
void Save(VariableManager* variableManager, Page *page, string path);
