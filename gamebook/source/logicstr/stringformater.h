#pragma once

#include <string>
#include "variablemanager.h"

using namespace std;

string StringSnakeToText(string source);
string ReplaceVariables(string source, VariableManager *variableManager);
