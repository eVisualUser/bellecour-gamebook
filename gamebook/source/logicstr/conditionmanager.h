#pragma once

#include <string>
#include <vector>

using namespace std;

class Condition {
public:
	string name;
	vector<string> list;
};

class ConditionManager {
public:
	void Load(string path);
	Condition GetCondition(string name);
	vector<Condition> actions;
};
