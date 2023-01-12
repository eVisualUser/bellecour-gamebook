#include "ui.h"
#include <stdexcept>
#include <sstream>

void UI::ResetBuffer() {
	this->buffer = vector<string>();
}

vector<string> UI::GenerateFrame() {
	if (this->buffer.empty())
		throw std::logic_error("Frame buffer empty");

	return this->buffer;
}

void UI::DrawRawLine(Point position, int lenght, char character) {
	if (lenght > 0) {
		for(int i = 0; i < lenght; i++) {
			position = position + Point(1, 0);
		}
	} else if (lenght == 0) {
		throw std::logic_error("Null raw line lenght drawing");
	} else {
		for(int i = 0; i > lenght; i--) {

		}
	}
}

void UI::DrawRawRect(Rect rect, char character) {

}

UI::UI(Point newSize) {
	this->size = newSize;

	for(int y = 0; y < newSize.y; y++) {
		stringstream stream;

		for (int x = 0; x < newSize.x; x++)
			stream << ' ';

		this->buffer.push_back(stream.str());
	}
}
