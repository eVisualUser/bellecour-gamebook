#include "page.h"

#include "../filesystem/reader.h"
#include "../filesystem/ini.h"
#include "../filesystem/toml.h"
#include "../logicstr/actionmanager.h"
#include "../logicstr/nodechain.h"
#include "../debug/logger.h"
#include "../render/console.h"

#include <iostream>
#include <iterator>
#include <sstream>

using namespace client_filesystem;

void Page::Load(string path) {
	auto reader = Reader();
	try {
		reader.SetPath(path);
		reader.ReadFile();
	} catch (string message) {
		Logger::LogError(message);
		PrintError(message);
		exit(-1);
	}

	stringstream logMessage;
	logMessage << "Open page: " << path;
	Logger::Log(logMessage.str());

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());

	auto config = ini.ParseTable("config");
	auto content = ini.ParseTable("content");
	auto choices = ini.ParseTable("choices");

	this->name = TomlParseString(config.GetVar("name").GetValue());
	this->type = TomlParseString(config.GetVar("type").GetValue());

	for (auto & var: content.GetAllVars()) {
		this->textContent.push_back(TomlParseString(var.GetValue()));
	}

	for (auto & var: choices.GetAllVars()) {
		auto button = Button();
		auto array = TomlParseArray(var.GetValue());

		button.text = TomlParseString(array[0]);
		button.condition = TomlParseString(array[1]);
		button.action = TomlParseString(array[2]);

		this->buttons.push_back(button);
	}
}

void Page::CreateButtons(InputManager *inputManager) {
	inputManager->ResetButtons();

	for(auto & button: this->buttons) {
		#ifdef __EMSCRIPTEN__
			inputManager->CreateButton(button.text);
		#else
			if (!inputManager->Exists(button.text))
				inputManager->CreateButton(button.text);
		#endif
	}
}

string Page::GetButtonPressed(string content, Executor *executor, ActionManager *actionManager, VariableManager *variableManager, ConditionManager *conditionManager) {
	string nextPage;
	for (auto & button: this->buttons) {
		if (content == button.text) {
			
			if (this->IsButtonActive(content, executor, variableManager, conditionManager)) {
				stringstream logMessage;
				logMessage = stringstream();
				logMessage << "Button pressed: " << button.text;
				Logger::Log(logMessage.str());

				auto nodeChain = NodeChain();
				auto actions = actionManager->GetAction(button.action);
				for (auto & action: actions.list) {
					NodeChain chain = NodeChain();
					chain.ParseString(action);

					auto nextPageBuffer = executor->ExecuteActionComand(variableManager, &chain);
					if (!nextPageBuffer.empty())
						nextPage = nextPageBuffer;
				}
			}
		}
	}
	return nextPage;
}

bool Page::IsButtonActive(string content, Executor *executor, VariableManager *variableManager, ConditionManager *conditionManager) {
	for (auto & button: this->buttons) {
		if (content == button.text) {
			bool conditionPassed = false;

			auto conditions = conditionManager->GetCondition(button.condition);
			for (auto & condition: conditions.list) {
				auto condNodeChain = NodeChain();
				condNodeChain.ParseString(condition);
				if (executor->ExecuteConditionComand(variableManager, &condNodeChain)) {
					conditionPassed = true;
				}
			}

			return conditionPassed;
		}
	}
	return false;
}
