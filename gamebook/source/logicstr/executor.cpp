#include "executor.h"
#include "nodechain.h"
#include "../debug/logger.h"

#include <iostream>
#include <stdexcept>

string Executor::ExecuteActionComand(VariableManager* variableManager, NodeChain* nodeChain) {
	string file;

	int buffer = 0;

	auto last_operator = Operator::None;
	for(auto & node: nodeChain->chain) {
		auto currentOp = nodeChain->GetOperator(node.content);
		if (currentOp != Operator::None) {
			if (currentOp == Operator::File) {
				for(auto & i: node.content) {
					if (i != '$') file.push_back(i);
				}
				return file;
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
			case Operator::None:
				buffer += value;
				break;
			default: case Operator::Equal:
				buffer = value;
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
	if (nodeChain->chain.size() == CONDITION_CHAIN_LEN) {

		int value_a = 0;
		auto first_node = nodeChain->chain[0].content;
		if (variableManager->IsExist(first_node)) {
			value_a = variableManager->GetVariableValue(first_node);
		} else {
			value_a = stoi(first_node);
		}

		int value_b = 0;
		auto last_node = nodeChain->chain[2].content;
		if (variableManager->IsExist(last_node)) {
			value_b = variableManager->GetVariableValue(last_node);
		} else {
			value_b = stoi(last_node);
		}

		auto nodeOpContent = nodeChain->chain[1].content;
		auto condOperator = nodeChain->GetOperator(nodeOpContent);
		switch(condOperator) {
			case Operator::Lower:
				return value_a < value_b;
			case Operator::Greater:
				return value_a > value_b;
			default: case Operator::Equal:
				return value_a == value_b;
		}
	} else {
		throw std::runtime_error(ERROR_MSG_INVALID_CHAIN_SIZE);
	}
}
