#include "logger.h"
#ifdef __EMSCRIPTEN__
#else
#include <ctime>
#include <fstream>
#include <sstream>
#endif

void Logger::AppendLine(string content) {
#ifdef __EMSCRIPTEN__
#else
  ofstream file;
  file.open(LOG_FILE_PATH, ios::app);
  file << content;
  file.close();
#endif
}

void Logger::Log(string message) {
#ifdef __EMSCRIPTEN__
#else
  stringstream ss;
  ss << Logger::GetStrTime() << ';' << "INFO" << ';' << message << ';' << endl;

  Logger::AppendLine(ss.str());
#endif
}

void Logger::LogError(string message) {
#ifdef __EMSCRIPTEN__
#else
  stringstream ss;
  ss << Logger::GetStrTime() << ';' << "ERROR" << ';' << message << ';' << endl;

  Logger::AppendLine(ss.str());
#endif
}

void Logger::LogWarning(string message) {
#ifdef __EMSCRIPTEN__
#else
  stringstream ss;
  ss << Logger::GetStrTime() << ';' << "WARNING" << ';' << message << ';'
     << endl;

  Logger::AppendLine(ss.str());
#endif
}

string Logger::GetStrTime() {
#ifdef __EMSCRIPTEN__
  return "UnCompatible";
#else
  time_t now = time(0);
  tm localTime;
  localtime_s(&localTime, &now);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
  return std::string(buffer);
#endif
}
