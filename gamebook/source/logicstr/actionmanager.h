#pragma once

#include <string>
#include <vector>

using namespace std;

class Action {
public:
	string name;
	vector<string> list;
};

class ActionManager {
public:
	void Load(string path);
	Action GetAction(string name);
	vector<Action> actions;
};
