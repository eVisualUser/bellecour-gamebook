#include "../include/error.hpp"

#if _WIN32 || _WIN64
#include <windows.h>
#else
#include <iostream>
#endif

#define CONSOLE_COLOR_DEFAULT cout << "\033[0m"
#define CONSOLE_COLOR_FG_RED cout << "\033[31m"

void Error::SetTitle(string newTitle) {
	this->title = newTitle;
}

void Error::SetDescription(string newDescription) {
	this->description = newDescription;
}

void Error::Show() {
	#if _WIN32 || _WIN64
		MessageBoxA(NULL, LPCSTR(this->description.c_str()), LPCSTR(this->title.c_str()), MB_OK | MB_ICONERROR);
	#else
		CONSOLE_COLOR_RED();
		cout << this->title << endl;
		cout << this->description << endl;
		CONSOLE_COLOR_DEFAULT();
	#endif
}

void Error::Active() {
	this->active = true;
}

void Error::Desactivate() {
	this->active = false;
}
