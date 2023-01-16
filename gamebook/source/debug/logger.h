#pragma once

#include <string>
#include <fstream>
#include <ctime>

using namespace std;

#define LOG_FILE_PATH "client.log"

class Logger {
public:
	static void AppendLine(string content);
	static void Log(string message);
	static void LogError(string message);
	static void LogWarning(string message);
private:
	static string GetStrTime();
};
