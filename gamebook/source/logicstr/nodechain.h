#pragma once

#include <string>
#include <vector>

using namespace std;

#define OPERATOR_SIGN_LOWER "<"
#define OPERATOR_SIGN_EQUAL "="
#define OPERATOR_SIGN_GREATER ">"
#define OPERATOR_SIGN_ADD "+"
#define OPERATOR_SIGN_SUB "-"
#define OPERATOR_SIGN_DIV "/"
#define OPERATOR_SIGN_MOD "%"
#define OPERATOR_SIGN_MULT "*"
#define OPERATOR_SIGN_FILE "$"

enum class Operator {
	Lower,
	Equal,
	Greater,
	Add,
	Sub,
	Div,
	Mod,
	Mult,
	File,
	None,
};

class Node {
public:
	string content;
	Operator type;
};

class NodeChain {
public:
	void ParseString(string input);
	enum Operator GetOperator(string input);
	Node GetFirst();
	vector<Node> chain;
};
