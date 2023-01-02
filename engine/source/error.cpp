#include "../include/Error.hpp"

#include <iostream>

#define SET_CONSOLE_COLOR_DEFAULT() cout << "\033[0m"
#define SET_CONSOLE_COLOR_FG_RED() cout << "\033[31m"

void Error::SetTitle(string newTitle) {
	this->title = newTitle;
}

void Error::SetMessage(string newMessage) {
	this->message = newMessage;
}

void Error::Show() {
	SET_CONSOLE_COLOR_FG_RED();
	cout << this->title << endl;
	cout << this->message << endl;
	SET_CONSOLE_COLOR_DEFAULT();
}

void Error::Active() {
	this->active = true;
}

void Error::Desactivate() {
	this->active = false;
}
