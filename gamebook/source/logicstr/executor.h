#pragma once

#include "variablemanager.h"
#include "nodechain.h"

class Executor {
public:
	/// Execute the command and update the variables.
	/// In the case of a goto file it will return the file.
	string ExecuteActionComand(VariableManager* variableManager, NodeChain* nodeChain);
};
