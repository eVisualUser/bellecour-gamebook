#pragma once

#include <string>
#include <vector>

#include "variablemanager.h"
#include "../logicstr/executor.h"

using namespace std;

string StringSnakeToText(string source);
string ReplaceVariables(string source, VariableManager *variableManager, Executor *executor);
bool TestInTextCondition(string list, VariableManager *variableManager, Executor *executor);
