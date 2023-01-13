#include "ui.h"
#include <ios>
#include <stdexcept>
#include <sstream>
#include <iostream>

void UI::ResetBuffer() {
	this->buffer = vector<string>();
	this->InitializeBuffer();
}

vector<string> UI::GetFrame() {
	if (this->buffer.empty())
		throw std::logic_error("Frame buffer empty");

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

void UI::DrawText(Point start, string text) {
	int lenght = std::strlen(text.c_str());
	for (int x = 0; x < lenght; x++) {
		this->buffer[start.y][start.x + x] = text[x];
	}
}

void UI::DrawButtons(Point position, ButtonManager *buttonManager) {
	#ifdef __EMSCRIPTEN__
	#else
	auto buttons = buttonManager->GetButtons();
	auto buttonPressed = buttonManager->GetLastPressed();

	for (int i = 0; i < buttonManager->GetButtonCount(); i++) {
		position.y += 2;
		if (i != buttonManager->GetIndex()) {
			this->DrawText(position, buttons[i]);
		} else {
			stringstream ss;
			ss << "> " << buttons[i];
			this->DrawText(position, ss.str());
		}
	}
	#endif
}
