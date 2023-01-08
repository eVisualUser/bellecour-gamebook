#include "executor.h"
#include "nodechain.h"
#include <iostream>
string Executor::ExecuteActionComand(VariableManager* variableManager, NodeChain* nodeChain) {
	string file;

	int buffer = 0;

	auto last_operator = Operator::None;
	for(auto & node: nodeChain->chain) {
		auto currentOp = nodeChain->GetOperator(node.content);
		if (currentOp != Operator::None) {
			if (currentOp == Operator::File) {
				for(auto & i: node.content) {
					cout << i << endl;
					if (i != '$') file.push_back(i);
					return file;
				}
			}
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
			variableManager->SetVariableValue(node.content, buffer);
		}
	}

	return file;
}

bool Executor::ExecuteConditionComand(VariableManager* variableManager, NodeChain* nodeChain) {

	auto op = Operator::None;

	int buffer = 0; 
	bool firstArg = true;;

	auto last_operator = Operator::None;
	for(auto & node: nodeChain->chain) {
		auto currentOp = nodeChain->GetOperator(node.content);

		switch(currentOp) {

		default: case Operator::None:
			if (firstArg) {
				if (variableManager->IsExist(node.content))
					buffer = variableManager->GetVariableValue(node.content);
				else
					buffer = stoi(node.content);

				firstArg = false;
			}	
			break;
		}

		last_operator = currentOp;

	}

	return false;
}