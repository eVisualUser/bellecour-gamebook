#include "ui.h"
#include "../debug/logger.h"

#include <ios>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <stdexcept>

void UI::ResetBuffer() {
  this->buffer = vector<string>();
  this->InitializeBuffer();
}

vector<string> UI::GetFrame() { return this->buffer; }

void UI::DrawLine(Point position, int length, char character) {
  if (length > 0) {
    position.x -= 1;
    for (int i = 0; i < length; i++) {
      position = position + Point(1, 0);
      if (position.x + 1 < this->buffer[0].length()) {
        this->buffer[position.y][position.x] = character;
      }
    }
  } else if (length == 0) {
  } else {
    for (int i = 0; i > length; i--) {
      position = position + Point(-1, 0);
      if (position.x - 1 > 0) {
        this->buffer[position.y][position.x] = character;
      }
    }
  }
}

UI::UI(Point newSize) {
  this->size = newSize;
  this->InitializeBuffer();
}

void UI::InitializeBuffer() {
  for (int y = 0; y < this->size.y; y++) {
    stringstream stream;

    for (int x = 0; x < this->size.x; x++)
      stream << ' ';

    this->buffer.push_back(stream.str());
  }
}

/*
fn text_return_line(source: String, lenght: i32) -> String {

    let mut result = String::new();

    let mut word = String::new();

    let mut count = 0_i32;

    for i in source.chars() {
        word.push(i);
        if i == ' ' {
            if (count + word.len() as i32) < lenght {
                result.push_str(&word);
                word = String::new();
            } else {
                count = 0;
                result.push('\n');
                result.push_str(&word);
                word = String::new();
            }
        }
        count += 1;
    }
    result.push_str(&word);

    result
}
*/
/*
int UI::DrawText(Point start, string text, VariableManager *variableManager,
                 Executor *executor) {
  int textBack = 1;
  if (text == "---") {
    this->DrawLine(start, this->size.x, '-');
  } else {
    text = ReplaceVariables(text, variableManager, executor);
    while (this->size.x < start.x + std::strlen(text.c_str()) && start.x > 0) {
      start.x--;
    }
    if (start.x < this->size.x && start.y < this->size.y && start.x >= 0 &&
        start.y >= 0) {
      int lenght = std::strlen(text.c_str());
      for (int x = 0; x < lenght; x++) {
        if ((start.x + x) < this->size.x) {
          this->buffer[start.y][start.x + x] = text[x];

        } else if (start.y + 1 < this->size.y) {
          int restLen = lenght - x;
          x -= 1;
          start.y += 1;
          start.x -= this->size.x;
        }
      }
    }
  }

  return textBack;
}
*/

void UI::UnsafeDrawTextAt(Point start, string text) {
  for (int i = 0; i <= text.length(); i++) {
    this->buffer[start.y][start.x + i] = text[i];
  }
}

int UI::DrawText(Point const start, string text, VariableManager *variableManager,
                 Executor *executor) {
  int textBack = 1;
  if (text == "---") {
    this->DrawLine(start, this->size.x, '-');
  } else {
    text = ReplaceVariables(text, variableManager, executor);

    int count = 1;
    string word;

    for (int i = 0; i < text.length(); i++) {
        word.push_back(text[i]);
        if (text[i] == ' ') {
          if (count + word.length() < this->size.x) {
            Point textPos = start;
            textPos.x -= word.length() - count;
            textPos.y += textBack;
            UnsafeDrawTextAt(textPos, word);
            word = "";
          } else {
            count = 0;
            textBack++;
            Point textPos = start;
            textPos.y += textBack;
            count += word.length();
            Logger::Log(word);
            UnsafeDrawTextAt(textPos, word);
            word = "";
          }
        }
        count++;
    }
    Point textPos = start;
    textPos.x -= word.length() - count;
    textPos.y += textBack;
    UnsafeDrawTextAt(textPos, word);
  }

  return textBack;
}

void UI::DrawButtons(Point position, InputManager *inputManager,
                     char selectedChar, VariableManager *variableManager,
                     Executor *executor) {
#ifdef __EMSCRIPTEN__
#else
  auto buttons = inputManager->GetButtons();
  auto buttonPressed = inputManager->GetLastPressed();

  for (int i = inputManager->GetButtonCount() - 1; i >= 0; i--) {
    position.y -= 2;
    if (i != inputManager->GetIndex()) {
      stringstream text;

      if (variableManager->IsExist("console_color_button_background")) {
        auto value = variableManager->GetVariableValue("console_color_button_background");
        text << "\033[" << variableManager->GetVariableValue("console_color_button_background") << "m";
      }
      if (variableManager->IsExist("console_color_button_foreground")) {
        text << "\033[" << variableManager->GetVariableValue("console_color_button_foreground") << "m";
      }

      text << buttons[i];

      if (variableManager->IsExist("console_color_foreground"))
        text << " \033[" << variableManager->GetVariableValue("console_color_foreground") << "m";
      else
        text << " \033[" << 32 << "m";

      if (variableManager->IsExist("console_color_background"))
        text << " \033[" << variableManager->GetVariableValue("console_color_background") << "m";
      else
        text << " \033[" << 40 << "m";

      this->DrawText(position, text.str(), variableManager, executor);
    } else {
      stringstream ss;

      stringstream text;

      if (selectedChar == '>' && variableManager->IsExist("console_color_selected_button_foreground")) {
        ss << "\033[" << variableManager->GetVariableValue("console_color_selected_button_foreground") << "m";
      }
      else if (selectedChar == 'X' && variableManager->IsExist("console_color_selected_button_locked_foreground"))
        ss << "\033[" << variableManager->GetVariableValue("console_color_selected_button_locked_foreground") << "m";

      if (selectedChar == '>' && variableManager->IsExist("console_color_selected_button_background"))
        ss << "\033[" << variableManager->GetVariableValue("console_color_selected_button_background") << "m";
      else if (selectedChar == 'X' && variableManager->IsExist("console_color_selected_button_locked_background"))
        ss << "\033[" << variableManager->GetVariableValue("console_color_selected_button_locked_background") << "m";

      ss << selectedChar << ' ' << buttons[i] ;

      if (variableManager->IsExist("console_color_foreground"))
        ss << " \033[" << variableManager->GetVariableValue("console_color_foreground") << "m";
      else
        ss << " \033[" << 32 << "m";

      if (variableManager->IsExist("console_color_background"))
        ss << " \033[" << variableManager->GetVariableValue("console_color_background") << "m";
      else
        ss << " \033[" << 40 << "m";

      this->DrawText(position, ss.str(), variableManager, executor);
    }
  }
#endif
}

void UI::Zoom() {
#ifdef __EMSCRIPTEN__
#else
  if ((this->size.x + 1) < INT_MAX && (this->size.y + 1) < INT_MAX) {
    this->size.x += 1;
    this->size.y += 1;
  }
#endif
}

void UI::UnZoom(int minX, int minY) {
#ifdef __EMSCRIPTEN__
#else
  if ((this->size.x - 1) > minX && (this->size.y - 1) > minY) {
    this->size.x -= 1;
    this->size.y -= 1;
  }
#endif
}
