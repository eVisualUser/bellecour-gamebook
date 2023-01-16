#include "logger.h"
#include <ctime>
#include <fstream>
#include <sstream>

void Logger::AppendLine(string content) {
	ofstream file;
	file.open(LOG_FILE_PATH, ios::app);
	file << content;
	file.close();
}

void Logger::Log(string message) {
	stringstream ss;
	ss << Logger::GetStrTime() << ';' << "LOG" << ';' << message << ';' << endl;

	Logger::AppendLine(ss.str());
}

void Logger::LogError(string message) {
	stringstream ss;
	ss << Logger::GetStrTime() << ';' << "ERROR" << ';' << message << ';' << endl;

	Logger::AppendLine(ss.str());
}

void Logger::LogWarning(string message) {
	stringstream ss;
	ss << Logger::GetStrTime() << ';' << "WARNING" << ';' << message << ';' << endl;

	Logger::AppendLine(ss.str());
}

string Logger::GetStrTime() {
	time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return std::string(buffer);
}
