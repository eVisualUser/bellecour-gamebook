#pragma once

#include <string>
#include <fstream>

using namespace std;

#define LOG_FILE_PATH "client.log"
#define LOG_FILE_OPEN_MODE "rw"

class Logger {
public:
	static void AppendLine(string content);
	static void Log(string message);
	static void LogError(string message);
	static void LogWarning(string message);
};
