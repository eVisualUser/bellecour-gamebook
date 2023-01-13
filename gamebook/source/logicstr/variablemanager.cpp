#include "variablemanager.h"

#include <iostream>

#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"

using namespace client_filesystem;

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

void VariableManager::Load(string path) {
	auto reader = Reader();
	reader.SetPath(path);
	reader.ReadFile();

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());
	auto table = ini.ParseTable("vars");

	for(auto & var: table.GetAllVars()) {
		Variable newVar;
		newVar.name = var.GetKey();
		newVar.value = TomlParseInt(var.GetKey());

		this->AddVariable(newVar);
	}
}
