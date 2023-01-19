#pragma once

#include <ctime>
#include <fstream>
#include <string>

using namespace std;

#define LOG_FILE_PATH "client.log"

class Logger {
public:
  static void AppendLine(string content);
  static void Log(string message);
  static void LogError(string message);
  static void LogWarning(string message);
  static string GetStrTime();
};
