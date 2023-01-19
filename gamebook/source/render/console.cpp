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
#ifdef __EMSCRIPTEN__
#else
  this->SetConsoleColor(COLOR_DEFAULT);
#endif
}

void Console::SetWindow() {
#ifdef __EMSCRIPTEN__
#else
  this->SetConsoleColor(COLOR_FOREGROUND_GREEN);
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
  for (auto &line : *frame) {
    buffer.append(line);
    buffer.push_back('\n');
  }
  std::cout << buffer << std::endl;
}

void PrintError(std::string error) {
  Logger::LogError(error);
#ifdef _WIN32
  MessageBox(NULL, LPCSTR(error.c_str()), LPCSTR("ERROR"),
             MB_ICONERROR | MB_OK);
#else
  std::cerr << "[ERROR] " << error << std::endl;
#endif
}

void Console::SetConsoleColor(int colorForeground) {
#ifdef __EMSCRIPTEN__
#elif _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, this->ConvertColor(colorForeground));
#else
  std::cout << "\033[" << colorForeground << 'm';
  std::cout << "\033[" << colorBackground << 'm';
#endif
}

int Console::ConvertColor(int ansiColor) {
#ifdef __EMSCRIPTEN__
#else
  switch (ansiColor) {
  case 30:
    return 0; // Black
  case 31:
    return FOREGROUND_RED; // Red
  case 32:
    return FOREGROUND_GREEN; // Green
  case 33:
    return FOREGROUND_RED | FOREGROUND_GREEN; // Yellow
  case 34:
    return FOREGROUND_BLUE; // Blue
  case 35:
    return FOREGROUND_RED | FOREGROUND_BLUE; // Magenta
  case 36:
    return FOREGROUND_GREEN | FOREGROUND_BLUE; // Cyan
  case 37:
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
  default:
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
  }
#endif
  return 0;
}
