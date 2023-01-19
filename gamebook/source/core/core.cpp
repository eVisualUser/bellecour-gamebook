#include "core.h"
#include "../filesystem/toml.h"
#include "../debug/logger.h"
#include "../render/console.h"
#include "../noise/noise.h"

#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Core::RunGameLoop() {
  while (true) {
    this->Update();
  }
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
#ifdef __EMSCRIPTEN__
#else
  char selectChar = '>';
  for (int i = 0; i < this->_inputManager.GetButtonCount(); i++) {
    if (!this->_page.IsButtonActive(this->_inputManager.GetButtons()[i],
                                    &this->_executor, &this->_variableManager,
                                    &this->_conditionManager)) {
      if (this->_inputManager.GetIndex() == i) {
        selectChar = 'X';
      }
    }
  }
  this->_ui.DrawButtons(Point(this->_ui.size.x / 2, this->_ui.size.y),
                        &this->_inputManager, selectChar);
#endif
  this->_ui.DrawText(Point(this->_ui.size.x / 2, 0), this->_page.name);

  int lineYOffset = 0;
  for (auto &line : this->_page.textContent) {
    lineYOffset +=
        1 + this->_ui.DrawText(Point(0,
                                     (this->_ui.size.y / 4) + lineYOffset),
                               line);
  }
}

void Core::Render() {
  auto frame = this->_ui.GetFrame();

  if (this->_variableManager.IsExist("text_noise_level")) {
    vector<char> chars;
    chars.push_back('a');
    chars.push_back('b');
    chars.push_back('d');
    chars.push_back('e');
    ApplyTextNoise(&frame, chars, this->_variableManager.GetVariableValue("text_noise_level") / 10);
  }

  this->_console.PrintFrame(&frame);
}

void Core::UpdateInputs() {
  this->_inputManager.Update();

  if (this->_inputManager.MustZoom())
    this->_ui.Zoom();
  else if (this->_inputManager.MustUnZoom())
    this->_ui.UnZoom();

  string nextPage = this->_page.GetButtonPressed(
      this->_inputManager.GetLastPressed(), &this->_executor,
      &this->_actionManager, &this->_variableManager, &this->_conditionManager);

  if (!nextPage.empty()) {
    this->_page = Page();

    stringstream stream;
    stream << "pages/" << nextPage;

    this->_page.Load(stream.str());

    this->_inputManager.ResetButtons();
    this->_page.CreateButtons(&this->_inputManager);
  }
}

void Core::LoadConfig() {
  auto reader = Reader();
  try {
    reader.SetPath(DEFAULT_CONFIG_PATH);
    reader.ReadFile();
  } catch(runtime_error message) {
    Logger::LogError(message.what());
    PrintError(message.what());
  }

  auto ini = client_filesystem::Ini();
  ini.SetBuffer(reader.GetBuffer());

  auto table = ini.ParseTable("client");
  this->_defaultPagePath = TomlParseString(table.GetVar("page").GetValue());
  this->_frameSize.x = TomlParseInt(table.GetVar("frameSize_x").GetValue());
  this->_frameSize.y = TomlParseInt(table.GetVar("frameSize_y").GetValue());
  this->_inputManager._keyOk = TomlParseInt(table.GetVar("ok").GetValue());
  this->_inputManager._keyExit = TomlParseInt(table.GetVar("exit").GetValue());
  this->_inputManager._keyUnZoom = TomlParseInt(table.GetVar("unzoom").GetValue());
  this->_inputManager._keyZoom = TomlParseInt(table.GetVar("zoom").GetValue());
  this->_inputManager._keyUp = TomlParseInt(table.GetVar("up").GetValue());
  this->_inputManager._keyDown = TomlParseInt(table.GetVar("down").GetValue());
}

void Core::Initialize() {
  this->_console = Console();
#ifdef __EMSCRIPTEN__
  this->_ui = UI(Point(80, 25));
#else
  this->_ui = UI(this->_frameSize);
#endif
  this->_ui.Zoom();

  this->_variableManager = VariableManager();
  this->_actionManager = ActionManager();
  this->_conditionManager = ConditionManager();

  this->_page = Page();

  this->_console.SetWindow();

  this->_variableManager.Load(DEFAULT_CONFIG_PATH);
  this->_actionManager.Load(DEFAULT_CONFIG_PATH);
  this->_conditionManager.Load(DEFAULT_CONFIG_PATH);

  this->_page.Load(this->_defaultPagePath);
  this->_page.CreateButtons(&this->_inputManager);
}
