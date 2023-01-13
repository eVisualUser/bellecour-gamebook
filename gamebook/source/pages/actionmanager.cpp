#include "actionmanager.h"

#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"

#include <iostream>

using namespace client_filesystem;

void ActionManager::Load(string path) {
	auto reader = Reader();
	reader.SetPath(path);
	reader.ReadFile();

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());
	auto table = ini.ParseTable("actions");

	for(auto & var: table.GetAllVars()) {
		Action newAction;
		newAction.list = TomlParseArray(var.GetValue());
		for (auto & action: newAction.list) {
			action = TomlParseString(action);
		}
		newAction.name = var.GetKey();
		this->actions.push_back(newAction);
	}
}

Action ActionManager::GetAction(string name) {
	for (auto & action: this->actions) {
		if (action.name == name)
			return action;
	}
	throw std::runtime_error("Action missing");
}
