#include "executor.h"
#include "nodechain.h"

string Executor::ExecuteActionComand(VariableManager* variableManager, NodeChain* nodeChain) {
	string file;

	int buffer = 0;

	auto last_operator = Operator::None;
	for(auto & node: nodeChain->chain) {
		auto currentOp = nodeChain->GetOperator(node.content);
		if (currentOp != Operator::None || currentOp == Operator::File) {
			
			if (currentOp == Operator::File)
				return node.content;
			last_operator = currentOp;
		} else {

			int value = 0;

			if (variableManager->IsExist(node.content))
				value = variableManager->GetVariableValue(node.content);
			else
				value = stoi(node.content);

			switch(last_operator) {
			case Operator::Add:
				buffer += value;
				break;
			case Operator::Sub:
				buffer -= value;
				break;
			case Operator::Div:
				buffer /= value;
				break;
			case Operator::Mod:
				buffer %= value;
				break;
			case Operator::Mult:
				buffer *= value;
				break;
			default: case Operator::None:
				buffer += value;
				break;
			}

			last_operator = Operator::None;
		}
	}

	for(auto & node: nodeChain->chain) {
		if (variableManager->IsExist(node.content)) {
			
		}
	}

	return file;
}
