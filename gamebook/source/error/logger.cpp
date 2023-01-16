#include "logger.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>

void Logger::AppendLine(string content) {
	ofstream file;
	file.open(LOG_FILE_PATH, ios::app);
	file << content;
	file.close();
}

void Logger::Log(string message) {
	stringstream ss;
	ss << "log = " << '\"' << message << '\"';

	Logger::AppendLine(ss.str());
}

void Logger::LogError(string message) {
	stringstream ss;
	ss << "error = " << '\"' << message << '\"';

	Logger::AppendLine(ss.str());
}

static void LogWarning(string message) {
	stringstream ss;
	ss << "warning = " << '\"' << message << '\"';

	Logger::AppendLine(ss.str());
}
