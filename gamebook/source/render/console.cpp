#include "console.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#else
	#include <conio.h>
#endif

#include "../debug/logger.h"

Console::Console() {
	// Optimize the console
	#ifdef _WIN32
		std::ios_base::sync_with_stdio(false);
		setvbuf(stdout, NULL, _IONBF, 0);
	#endif
}

Console::~Console() {
	#ifdef _WIN32
		HWND hwnd = GetConsoleWindow();
		
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hwnd, &consoleInfo);
		WORD defaultAttribute = consoleInfo.wAttributes;
		SetConsoleTextAttribute(hwnd, defaultAttribute);

		std::cout << COLOR_DEFAULT << std::endl;
	#elif __EMSCRIPTEN__
	#else
		std::cout << COLOR_DEFAULT << std::endl;
	#endif
}

void Console::SetWindow(int x, int y) {
	#ifdef _WIN32
		HWND hwnd = GetConsoleWindow();

		WORD colorAttribute = FOREGROUND_GREEN;
		SetConsoleTextAttribute(hwnd, colorAttribute);

		std::cout << COLOR_FOREGROUND_GREEN << COLOR_BACKGROUND_BLACK << std::endl;
	#elif __EMSCRIPTEN__
	#else
		std::cout << COLOR_FOREGROUND_GREEN << COLOR_BACKGROUND_BLACK << std::endl;

		struct winsize w;
		w.ws_row = x;
		w.ws_col = y;
		ioctl(STDOUT_FILENO, TIOCSWINSZ, &w);
	#endif
}

void Console::WaitAny() {
	#ifdef __EMSCRIPTEN__
	#else
	_getch();
	#endif
}

void Console::Clear() {
	#ifdef __EMSCRIPTEN__
		emscripten_run_script("document.getElementById(\"output\").value = \"\";");
	#elif _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void Console::PrintFrame(std::vector<std::string> *frame) {
	std::string buffer;
	for (auto & line: *frame) {
		buffer.append(line);
		buffer.push_back('\n');
	}
	std::cout << buffer << std::endl;
}

void PrintError(std::string error) {
	Logger::LogError(error);
	#ifdef _WIN32
		MessageBox(NULL, LPCSTR(error.c_str()), LPCSTR("ERROR"), MB_ICONERROR | MB_OK);
	#else
		std::cerr << "[ERROR] " << error << std::endl;
	#endif
}
