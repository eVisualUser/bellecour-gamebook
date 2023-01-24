#pragma once

#define CONDITION_CHAIN_LEN 3
#define ERROR_MSG_INVALID_CHAIN_SIZE                                           \
  "Any condition must be composed of 3 node: node_1 > node_2"
#define ERROR_MSG_UNKNOW_OPERATOR "Unknow operator"

#include "nodechain.h"
#include "variablemanager.h"

class Executor {
public:
  /// Execute the command and update the variables.
  /// In the case of a goto file it will return the file.
  string ExecuteActionComand(VariableManager *variableManager,
                             NodeChain *nodeChain);
  bool ExecuteConditionComand(VariableManager *variableManager,
                              NodeChain *nodeChain);
};
