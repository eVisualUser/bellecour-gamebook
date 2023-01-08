#include "nodechain.h"

#include <iostream>

void NodeChain::ParseString(string input) {
	string buffer;
	for(auto & i: input) {
		if (i != ' ')
			buffer.push_back(i);
		else {
			Node node;
			node.content = buffer;
			node.type = this->GetOperator(buffer);
			this->chain.push_back(node);
			buffer = "";
		}
	}
	Node node;
	node.content = buffer;
	node.type = this->GetOperator(buffer);
	this->chain.push_back(node);
}

Operator NodeChain::GetOperator(string input) {

	if (input == OPERATOR_SIGN_ADD)
		return Operator::Add;
	else if (input == OPERATOR_SIGN_DIV)
		return Operator::Div;
	else if (input == OPERATOR_SIGN_EQUAL)
		return Operator::Equal;
	else if (input == OPERATOR_SIGN_MULT)
		return Operator::Mult;
	else if (input == OPERATOR_SIGN_SUB)
		return Operator::Sub;
	else if (input == OPERATOR_SIGN_MOD)
		return Operator::Mod;
	else if (input == OPERATOR_SIGN_GREATER)
		return Operator::Greater;
	else if (input[0] == OPERATOR_SIGN_FILE)
		return Operator::File;
	else if (input == OPERATOR_SIGN_LOWER)
		return Operator::Lower;
	else
		return Operator::None;

	auto message = "Unkow operator";
	cerr << message << endl;
	throw std::runtime_error(message);
}

Node NodeChain::GetFirst() {
	return this->chain[0];
}
