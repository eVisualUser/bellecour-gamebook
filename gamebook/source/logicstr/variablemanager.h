#pragma once

#include <string>
#include <vector>

using namespace std;

struct Variable {
	string name;
	int value;
};

class VariableManager {
public:
	void Load(string path);
	void CreateVariable(string name, int value);
	void AddVariable(Variable variable);
	int GetVariableValue(string name);
	bool IsExist(string name);
	void SetVariableValue(string name, int value);
    vector<Variable>  GetAllVariables();
private:
	vector<Variable> buffer;
};
