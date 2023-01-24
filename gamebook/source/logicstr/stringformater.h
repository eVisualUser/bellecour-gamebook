#pragma once

#include <string>
#include <vector>

#include "../logicstr/executor.h"
#include "variablemanager.h"

using namespace std;

string StringSnakeToText(string source);
string ReplaceVariables(string source, VariableManager *variableManager,
                        Executor *executor);
bool TestInTextCondition(string list, VariableManager *variableManager,
                         Executor *executor);
