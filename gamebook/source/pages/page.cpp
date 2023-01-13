#include "page.h"

#include "../filesystem/reader.h"
#include "../filesystem/ini.h"
#include "../filesystem/toml.h"
#include "actionmanager.h"
#include "../logicstr/nodechain.h"

#include <iostream>
#include <iterator>

using namespace client_filesystem;

void Page::Load(string path) {
	auto reader = Reader();
	reader.SetPath(path);
	reader.ReadFile();

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

void Page::CreateButtons(ButtonManager *buttonManager) {
	buttonManager->ResetButtons();

	for(auto & button: this->buttons) {
		#ifdef __EMSCRIPTEN__
			buttonManager->CreateButton(button.text);
		#else
			if (!buttonManager->Exists(button.text))
				buttonManager->CreateButton(button.text);
		#endif
	}
}

string Page::GetButtonPressed(string content, Executor *executor, ActionManager *actionManager, VariableManager *variableManager) {
	string nextPage;
	try {
	for (auto & button: this->buttons) {
		if (content == button.text) {
			auto nodeChain = NodeChain();
			auto actions = actionManager->GetAction(button.action);
			for (auto & unit: actions.list) {
				NodeChain chain = NodeChain();
				chain.ParseString(unit);

				auto nextPageBuffer = executor->ExecuteActionComand(variableManager, &chain);
				if (!nextPageBuffer.empty()) {
					nextPage = nextPageBuffer;
				}
			}
		}
	}
	} catch(const string message) {
		cout << message << endl;
	}
	return nextPage;
}
