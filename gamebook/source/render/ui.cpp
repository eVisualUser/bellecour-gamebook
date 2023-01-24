#include "ui.h"
#include "../debug/logger.h"

#include <ios>
#include <limits.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

void UI::ResetBuffer() {
	this->buffer = vector<string>();
	this->InitializeBuffer();
}

vector<string> UI::GetFrame() {
	return this->buffer;
}

void UI::DrawRawLine(Point position, int length, char character) {
	if (length > 0) {
		position.x -= 1;
		for(int i = 0; i < length; i++) {
			position = position + Point(1, 0);
			if (position.x < this->buffer[0].length()) {
				this->buffer[position.y][position.x] = character;
			}
		}
	} else if (length == 0) {
	} else {
		for(int i = 0; i > length; i--) {
			position = position + Point(-1, 0);
			if (position.x > 0) {
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
	for(int y = 0; y < this->size.y; y++) {
		stringstream stream;

		for (int x = 0; x < this->size.x; x++)
			stream << ' ';

		this->buffer.push_back(stream.str());
	}
}

int UI::DrawText(Point start, string text, VariableManager* variableManager, Executor *executor) {
    text = ReplaceVariables(text, variableManager, executor);
	int textBack = 1;
        int startBack = 0;
        while(this->size.x < start.x + std::strlen(text.c_str()) && start.x > 0) {
          start.x--;
          startBack++;
        }
        if (start.x < this->size.x && start.y < this->size.y && start.x >= 0 && start.y >= 0) {
		int lenght = std::strlen(text.c_str());
		for (int x = 0; x < lenght; x++) {
			if ((start.x + x) < this->size.x)
				this->buffer[start.y][start.x + x] = text[x];
                        else if (start.y + 1 < this->size.y) {
                          int restLen = lenght - x;
                          x -= 1;
                          textBack++;
                          start.y += 1;
                          start.x -= this->size.x;
                        }
                }
        }

	return textBack;
}

void UI::DrawButtons(Point position, InputManager *inputManager, char selectedChar, VariableManager* variableManager, Executor *executor) {
	#ifdef __EMSCRIPTEN__
	#else
	auto buttons = inputManager->GetButtons();
	auto buttonPressed = inputManager->GetLastPressed();

	for (int i = inputManager->GetButtonCount() - 1; i >= 0; i--) {
		position.y -= 2;
		if (i != inputManager->GetIndex()) {
                  this->DrawText(position, buttons[i], variableManager, executor);
		} else {
			stringstream ss;
			ss << selectedChar << ' ' << buttons[i];
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

void UI::UnZoom() {
#ifdef __EMSCRIPTEN__
#else
  if ((this->size.x - 1) > 25 && (this->size.y - 1) > 25) {
			this->size.x -= 1;
			this->size.y -= 1;
		}
	#endif
}
