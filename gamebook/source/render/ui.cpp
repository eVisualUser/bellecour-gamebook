#include "ui.h"
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

void UI::DrawRawRect(Rect rect, char character) {
	if (rect.min.x >= 0 && rect.min.y >= 0) {
		if (rect.max.x < this->size.x && rect.max.y < this->size.y) {
			// Top Line
			this->DrawRawLine(rect.min, rect.max.x, character);
			// Bottom Line
			this->DrawRawLine(rect.min + rect.max, -(rect.max.x), character);

			auto rightStart = rect.min;
			auto leftStart = rect.min;
			leftStart.x = rect.max.x;

			for (int y = 0; y < rect.max.y; y++) {
				this->buffer[rightStart.x][rightStart.y + y] = character;
				this->buffer[leftStart.x][leftStart.y + y] = character;
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

int UI::DrawText(Point start, string text) {
	int textBack = 0;
	if (start.x < this->size.x && start.y < this->size.y) {
		int lenght = std::strlen(text.c_str());
		for (int x = 0; x < lenght; x++) {
			if ((start.x + x) < this->size.x)
				this->buffer[start.y][start.x + x] = text[x];
			else if (start.y + 1 < this->size.y) {
				x -= 1;
				textBack++;
				start.y += 1;
				start.x -= x;
				start.x -= 1;
			}
		}
	}
	return textBack;
}

void UI::DrawButtons(Point position, InputManager *inputManager) {
	#ifdef __EMSCRIPTEN__
	#else
	auto buttons = inputManager->GetButtons();
	auto buttonPressed = inputManager->GetLastPressed();

	for (int i = inputManager->GetButtonCount() - 1; i >= 0; i--) {
		position.y -= 2;
		if (i != inputManager->GetIndex()) {
			this->DrawText(position, buttons[i]);
		} else {
			stringstream ss;
			ss << "> " << buttons[i];
			this->DrawText(position, ss.str());
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
		if ((this->size.x - 1) > 5 && (this->size.y - 1) > 5) {
			this->size.x -= 1;
			this->size.y -= 1;
		}
	#endif
}
