#include "conditionmanager.h"

#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../render/console.h"
#include "../debug/logger.h"
#include <sstream>

using namespace client_filesystem;

void ConditionManager::Load(string path) {
	auto reader = Reader();
	try {
		reader.SetPath(path);
		reader.ReadFile();
	} catch (string message) {
		Logger::LogError(message);
		PrintError(message);
	}

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());
	auto table = ini.ParseTable("conditions");

	for(auto & var: table.GetAllVars()) {
		Condition condition;
		condition.list = TomlParseArray(var.GetValue());
		for (auto & condition: condition.list) {
			condition = TomlParseString(condition);
		}
		condition.name = var.GetKey();
		this->conditions.push_back(condition);
	}
}

Condition ConditionManager::GetCondition(string name) {
	for (auto & condition: this->conditions) {
		if (condition.name == name)
			return condition;
	}
	stringstream message;
	message << "Condition Missing: " << name; 
	Logger::LogError(message.str());
	throw std::runtime_error(message.str());
}
