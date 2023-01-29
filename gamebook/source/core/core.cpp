#include "core.h"
#include "../debug/logger.h"
#include "../filesystem/ini.h"
#include "../filesystem/reader.h"
#include "../filesystem/toml.h"
#include "../logicstr/stringformater.h"
#include "../logicstr/variablemanager.h"
#include "../noise/noise.h"
#include "../render/console.h"
#include "../save/save.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Core::RunGameLoop() {
  while (true) {
    this->Update();
  }
}

void Core::SpecialPages() {
  if (this->_page.type == "Quit") {
#ifdef __EMSCRIPTEN__
#else
    stringstream lastFileMessage;
    lastFileMessage << "Last page: " << this->_lastPage;
    Logger::Log(lastFileMessage.str());
    Logger::Log("Game Quit");
    exit(0);
#endif
  } else if (this->_page.type == "Reset") {
    this->_variableManager = VariableManager();
    this->_variableManager.Load("config.toml");
  } else if (this->_page.type == "Save") {
#ifdef __EMSCRIPTEN__
#else
    string buttonName = "[SAVE] Save";

    bool exist = false;
    for (auto &button : this->_inputManager.GetButtons()) {
      if (button == buttonName)
        exist = true;
      else if (button.contains("[SAVE]"))
        this->_inputManager.Remove(button);
    }

    if (!exist)
      this->_inputManager.CreateButton(buttonName, &this->_variableManager,
                                       &this->_executor);

    if (this->_inputManager.GetLastPressed() == buttonName)
      Save(&this->_variableManager, &this->_page, "saves.toml");
#endif
  } else if (this->_page.type == "LoadSave") {
#ifdef __EMSCRIPTEN__
#else
    auto reader = Reader();
    try {
      reader.SetPath("saves.toml");
      reader.ReadFile();
    } catch (runtime_error error) {
      stringstream lastFileMessage;
      lastFileMessage << "Last page: " << this->_lastPage;
      Logger::LogError(lastFileMessage.str());
      Logger::LogError(error.what());
      PrintError(error.what());
      exit(-1);
    }

    auto savesFile = Ini();
    savesFile.SetBuffer(reader.GetBuffer());

    for (auto &saveName : savesFile.GetAllTables()) {
      stringstream buttonName;
      buttonName << "[SAVE] " << saveName;

      bool exist = false;
      for (auto &button : this->_inputManager.GetButtons()) {
        if (button == buttonName.str())
          exist = true;
      }
      if (!exist)
        this->_inputManager.CreateButton(
            buttonName.str(), &this->_variableManager, &this->_executor);
      if (this->_inputManager.GetLastPressed() == buttonName.str()) {
        LoadSave(&this->_variableManager, &this->_page, "saves.toml", saveName);
        this->_inputManager.ResetButtons();
        this->_page.CreateButtons(&this->_inputManager, &this->_variableManager,
                                  &this->_executor);
      }
    }
#endif
  }
}

void Core::Update() {
  this->SpecialPages();
  this->ClearScreen();
  this->UpdateSpecialVariables();
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
      if (this->_inputManager.GetIndex() == i &&
          !(this->_inputManager.GetButtons()[this->_inputManager.GetIndex()]
                .contains("[SAVE]"))) {
        selectChar = 'X';
      }
    }
  }

  this->_ui.DrawButtons(Point(this->_ui.size.x / 2, this->_ui.size.y),
                        &this->_inputManager, selectChar,
                        &this->_variableManager, &this->_executor);
#endif
  this->_ui.DrawText(Point(this->_ui.size.x / 2, 0), this->_page.name,
                     &this->_variableManager, &this->_executor);

  int lineYOffset = 0;
  for (auto &line : this->_page.textContent) {
    lineYOffset +=
        1 + this->_ui.DrawText(Point(0, (this->_ui.size.y / 4) + lineYOffset),
                               line, &this->_variableManager, &this->_executor);
  }

  if (this->_page.type == "Recap") {
    if (this->_page.fileName != this->_lastPage) {
      string fileNameClearedbuffer;
      bool beforeSlash = true;
      bool afterDot = false;
      for (auto & i: this->_lastPage) {
          if (i == '/')
            beforeSlash = false;
          else if (i == '.')
            afterDot = true;
          else if (!beforeSlash && !afterDot) {
            fileNameClearedbuffer.push_back(i);
          }
      }

      stringstream lastPage;
      lastPage << "Last Page: " << fileNameClearedbuffer;
      this->_ui.DrawText(Point(0, (this->_ui.size.y / 4) + ++lineYOffset),
                             lastPage.str(), &this->_variableManager,
                             &this->_executor);
  }

    for (auto &var : this->_variableManager.GetAllVariables()) {
      if (var.name.contains("recap_bool_")) {
        string buffer;
        for (int i = strlen("recap_bool_"); i < var.name.length(); i++)
          buffer.push_back(var.name[i]);

        stringstream text;
        text << StringSnakeToText(buffer);
        text << ": ";

        if (var.value == 0)
          text << "True";
        else
          text << "False";

        this->_ui.DrawText(Point(0, (this->_ui.size.y / 4) + ++lineYOffset),
                           text.str(), &this->_variableManager,
                           &this->_executor);
      }
      if (var.name.contains("recap_")) {
        string buffer;
        for (int i = strlen("recap_"); i < var.name.length(); i++)
          buffer.push_back(var.name[i]);

        stringstream text;
        text << StringSnakeToText(buffer);
        text << ": ";
        text << var.value;

        this->_ui.DrawText(Point(0, (this->_ui.size.y / 4) + ++lineYOffset),
                           text.str(), &this->_variableManager,
                           &this->_executor);
      }
    }
  }
}

void Core::UpdateSpecialVariables() {
  if (this->_variableManager.IsExist("console_color_foreground")) {
    this->_console.SetConsoleColor(
        this->_variableManager.GetVariableValue("console_color_foreground"));
  }
  if (this->_variableManager.IsExist("console_color_background")) {
    this->_console.SetConsoleColor(
        this->_variableManager.GetVariableValue("console_color_background"));
  }
  if (this->_variableManager.IsExist("console_color_foreground_karma_up") &&
      this->_variableManager.IsExist("karma") &&
      this->_variableManager.GetVariableValue("karma") >= 0) {
    this->_console.SetConsoleColor(this->_variableManager.GetVariableValue(
        "console_color_foreground_karma_up"));
  }
  if (this->_variableManager.IsExist("console_color_foreground_karma_down") &&
      this->_variableManager.IsExist("karma") &&
      this->_variableManager.GetVariableValue("karma") < 0) {
    this->_console.SetConsoleColor(this->_variableManager.GetVariableValue(
        "console_color_foreground_karma_down"));
  }
  if (this->_variableManager.IsExist("console_color_background_karma_down") &&
      this->_variableManager.IsExist("karma") &&
      this->_variableManager.GetVariableValue("karma") < 0) {
    this->_console.SetConsoleColor(this->_variableManager.GetVariableValue(
        "console_color_background_karma_down"));
  }
  if (this->_variableManager.IsExist("console_color_background_karma_up") &&
      this->_variableManager.IsExist("karma") &&
      this->_variableManager.GetVariableValue("karma") >= 0) {
    this->_console.SetConsoleColor(this->_variableManager.GetVariableValue(
        "console_color_background_karma_up"));
  }
}

void Core::Render() {
  auto frame = this->_ui.GetFrame();

  if (this->_variableManager.IsExist("text_noise_level")) {
    vector<char> chars;
    chars.push_back('a');
    chars.push_back('i');
    chars.push_back('o');
    chars.push_back('e');
    chars.push_back('u');
    ApplyTextNoise(&frame, chars,
                   this->_variableManager.GetVariableValue("text_noise_level") /
                       10);
  }

#ifdef __EMSCRIPTEN__
  for (auto &button : this->_page.GetButtons()) {
    auto text = ReplaceVariables(button.text, &this->_variableManager,
                                 &this->_executor);
    if (!this->_page.IsButtonActive(text, &this->_executor,
                                    &this->_variableManager,
                                    &this->_conditionManager)) {
      stringstream jsCommand;
      jsCommand << "desactivateButton(";
      jsCommand << '\"' << text << '\"';
      jsCommand << ");";
      emscripten_run_script(jsCommand.str().c_str());
    }
  }
#endif

  this->_console.PrintFrame(&frame);
}

void Core::UpdateInputs() {
  if (this->_inputManager.Update()) {
    auto filePathBuffer = this->_page.fileName;
    this->_page = Page();
    this->_page.Load(filePathBuffer);
  }

  if (this->_inputManager.MustZoom())
    this->_ui.Zoom();
  else if (this->_inputManager.MustUnZoom())
    this->_ui.UnZoom(this->_minUnZoomX, this->_minUnZoomY);

  string nextPage = this->_page.GetButtonPressed(
      this->_inputManager.GetLastPressed(), &this->_executor,
      &this->_actionManager, &this->_variableManager, &this->_conditionManager);

  if (!nextPage.empty()) {
    stringstream lastFileMessage;
    lastFileMessage << "Last page: " << this->_lastPage;
    Logger::Log(lastFileMessage.str());

    if (!this->_page.fileName.contains("recap"))
      this->_lastPage = this->_page.fileName;

    this->_page = Page();

    stringstream stream;
    stream << "pages/" << nextPage;

    this->_page.Load(stream.str());

    this->_inputManager.ResetButtons();
    this->_page.CreateButtons(&this->_inputManager, &this->_variableManager,
                              &this->_executor);
  }
}

void Core::LoadConfig() {
  auto reader = Reader();
  try {
    reader.SetPath(this->_defaultConfigPath);
    reader.ReadFile();
  } catch (runtime_error message) {
    stringstream lastFileMessage;
    lastFileMessage << "Last page: " << this->_lastPage;
    Logger::LogError(lastFileMessage.str());
    Logger::LogError(message.what());
    PrintError(message.what());
  }

  auto ini = Ini();
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
  auto var_frame_size_min_x = table.GetVar("frame_size_min_x");
  if (!var_frame_size_min_x.GetKey().empty()) {
    this->_minUnZoomX = TomlParseInt(var_frame_size_min_x.GetValue());
  }
  auto var_frame_size_min_y = table.GetVar("frame_size_min_y");
  if (!var_frame_size_min_y.GetKey().empty()) {
    this->_minUnZoomY = TomlParseInt(var_frame_size_min_y.GetValue());
  }
}

void Core::Initialize() {
  this->_console = Console();
#ifdef __EMSCRIPTEN__
  this->_ui = UI(Point(75, 25));
#else
  this->_ui = UI(this->_frameSize);
#endif

  this->_variableManager = VariableManager();
  this->_actionManager = ActionManager();
  this->_conditionManager = ConditionManager();

  this->_page = Page();

  this->_console.SetWindow();

#ifdef __EMSCRIPTEN__
#else
  ofstream savesFile("saves.toml", ios::app);
  savesFile.close();
#endif

  this->_variableManager.Load(this->_defaultConfigPath);
  this->_actionManager.Load(this->_defaultConfigPath);
  this->_conditionManager.Load(this->_defaultConfigPath);

  this->_page.Load(this->_defaultPagePath);
  this->_page.CreateButtons(&this->_inputManager, &this->_variableManager,
                            &this->_executor);
}
