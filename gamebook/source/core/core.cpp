#include "core.h"

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

void Core::RunGameLoop() {
	while(true) {this->Update();}
}

void Core::Update() {
	this->ClearScreen();
	this->Draw();
	this->Render();
	this->UpdateInputs();
}

void Core::ClearScreen() {
	this->_ui.ResetBuffer();
	this->_console.Clear();
}

void Core::Draw() {
	this->_ui.DrawButtons(Point(15, 10), &this->_buttonManager);
	this->_ui.DrawText(Point(5, 0), this->_page.name);

	for (auto & line: this->_page.textContent)
			this->_ui.DrawText(Point(0, 5), line);
}

void Core::Render() {
	auto frame = this->_ui.GetFrame();
	this->_console.PrintFrame(&frame);
}

void Core::UpdateInputs() {
	this->_buttonManager.Update();

	string nextPage = this->_page.GetButtonPressed(
			this->_buttonManager.GetLastPressed(),
			 &this->_executor,
			  &this->_actionManager,
			   &this->_variableManager);

	if (!nextPage.empty()) {
		this->_page = Page();

		stringstream stream;
		stream << "pages/" << nextPage;

		this->_page.Load(stream.str());
		
		this->_buttonManager.ResetButtons();
		this->_page.CreateButtons(&this->_buttonManager);
	}
}

void Core::LoadConfig(string path) {
	auto reader = Reader();
	reader.SetPath(path);
	reader.ReadFile();

	auto ini = client_filesystem::Ini();
	ini.SetBuffer(reader.GetBuffer());

	auto tableTable = ini.ParseTable("default_path");
	this->_defaultConfigPath = tableTable.GetVar("config").GetValue();
	this->_defaultPagePath = tableTable.GetVar("page").GetValue();

	auto renderTable = ini.ParseTable("render");
	this->_frameSize.x = stoi(renderTable.GetVar("frameSize_x").GetValue());
	this->_frameSize.y = stoi(renderTable.GetVar("frameSize_y").GetValue());
	this->_textPosition.x = stoi(renderTable.GetVar("textPosition_x").GetValue());
	this->_textPosition.y = stoi(renderTable.GetVar("textPosition_y").GetValue());
	this->_buttonPosition.x = stoi(renderTable.GetVar("buttonPosition_x").GetValue());
	this->_buttonPosition.y = stoi(renderTable.GetVar("buttonPosition_y").GetValue());
}

void Core::Initialize() {
	this->_console = Console();
	this->_ui = UI(this->_frameSize);

	this->_variableManager = VariableManager();
	this->_actionManager = ActionManager();
	this->_conditionManager = ConditionManager();

	this->_page = Page();

	this->_console.SetWindow(600, 600);

	this->_variableManager.Load(this->_defaultConfigPath);
	this->_actionManager.Load(this->_defaultConfigPath);
	this->_conditionManager.Load(this->_defaultConfigPath);
	
	this->_page.Load(this->_defaultPagePath);
	this->_page.CreateButtons(&this->_buttonManager);
}
