#include "conditionmanager.h"

#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"

#include <iostream>

using namespace client_filesystem;

void ConditionManager::Load(string path) {
	auto reader = Reader();
	reader.SetPath(path);
	reader.ReadFile();

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());
	auto table = ini.ParseTable("actions");

	for(auto & var: table.GetAllVars()) {
		Condition condition;
		condition.list = TomlParseArray(var.GetValue());
		for (auto & condition: condition.list) {
			condition = TomlParseString(condition);
		}
		condition.name = var.GetKey();
		this->actions.push_back(condition);
	}
}

Condition ConditionManager::GetCondition(string name) {
	for (auto & condition: this->actions) {
		if (condition.name == name)
			return condition;
	}
	throw std::runtime_error("Action missing");
}
