#include "variablemanager.h"

#include <iostream>

void VariableManager::CreateVariable(string name, int value) {
	Variable variable = {
		.name = name,
		.value = value,
	};

	this->AddVariable(variable);
}

void VariableManager::AddVariable(Variable variable) {
	this->buffer.push_back(variable);
}

int VariableManager::GetVariableValue(string name) {
	for(auto & i: this->buffer)
		if (i.name == name) return i.value;

	auto message = "Unknow variable";
	cerr << message << endl;
	throw std::runtime_error(message);
}

bool VariableManager::IsExist(string name) {
	for(auto & var: this->buffer) {
		if (var.name == name)
			return true;
	}
	return false;
}

void VariableManager::SetVariableValue(string name, int value) {
	for(int i = 0; i < this->buffer.size(); i++) {
		if (this->buffer[i].name == name) {
			this->buffer[i].value = value;
			break;
		}
	}
}
