#include "console.h"
#include <string>

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

#include <sstream>

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
  std::stringstream script;
  if (colorForeground >= 40) {
    script << "changeTextAreaColorBackground(" << colorForeground
           << ");"; // Background
  } else {
    script << "changeTextAreaColor(" << colorForeground << ");"; // Foreground
  }
  emscripten_run_script(script.str().c_str());
#else
  std::cout << "\033[" << colorForeground << 'm';
#endif
}
// changeTextAreaColorBackground
int Console::ConvertColor(int ansiColor) {
#ifdef __EMSCRIPTEN__
#elif _WIN32
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
  case 41:
    return BACKGROUND_RED; // Background Red
  case 42:
    return BACKGROUND_GREEN; // Background Green
  case 43:
    return BACKGROUND_RED | BACKGROUND_GREEN; // Background Yellow
  case 44:
    return BACKGROUND_BLUE; // Background Blue
  case 45:
    return BACKGROUND_RED | BACKGROUND_BLUE; // Background Magenta
  case 46:
    return BACKGROUND_GREEN | BACKGROUND_BLUE; // Background Cyan
  case 47:
    return BACKGROUND_RED | BACKGROUND_GREEN |
           BACKGROUND_BLUE; // Background White
  default:
    return BACKGROUND_GREEN; // White
  }
#endif
  return 0;
}

void gotoxy(short  x, short y) {
  COORD pos = {x, y};
  HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(output, pos);
}

void Console::SetColorAt(Point position, int color) {
#ifdef __EMSCRIPTEN__
#elif _WIN32
    gotoxy(position.x, position.y);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, this->ConvertColor(color));
    gotoxy(0, 0);
#endif
}
