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
	this->_ui.DrawButtons(Point(this->_ui.size.x / 3, this->_ui.size.y), &this->_inputManager);
	this->_ui.DrawText(Point(this->_ui.size.x / 2, 0), this->_page.name);

	int lineYOffset = 0;
	for (auto & line: this->_page.textContent) {
			lineYOffset += 1 + this->_ui.DrawText(Point(this->_ui.size.x / 4, (this->_ui.size.y / 4) + lineYOffset), line);
	}
}

void Core::Render() {
	auto frame = this->_ui.GetFrame();
	this->_console.PrintFrame(&frame);
}

void Core::UpdateInputs() {
	this->_inputManager.Update();

	if (this->_inputManager.MustZoom())
		this->_ui.Zoom();
	else if (this->_inputManager.MustUnZoom())
		this->_ui.UnZoom();

	string nextPage = this->_page.GetButtonPressed(
			this->_inputManager.GetLastPressed(),
			 &this->_executor,
			  &this->_actionManager,
			   &this->_variableManager,
			   &this->_conditionManager);

	if (!nextPage.empty()) {
		this->_page = Page();

		stringstream stream;
		stream << "pages/" << nextPage;

		this->_page.Load(stream.str());
		
		this->_inputManager.ResetButtons();
		this->_page.CreateButtons(&this->_inputManager);
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
	this->_page.CreateButtons(&this->_inputManager);
}
