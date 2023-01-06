#include "reader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <system_error>

#define ERROR_FILE_NOT_FOUND() throw std::runtime_error("File not found");

using namespace client_filesystem;

bool Reader::IsFileExist() {
	ifstream file;
	file.open(this->path);

	if (!file) {
		file.close();
		return false;
	}

	file.close();
	return true;
}

void Reader::SetPath(string newPath) {
	this->path = newPath;
	if (!this->IsFileExist())
	{
		cerr << "File not found: " << "\"" << this->path << "\"" << endl;
		ERROR_FILE_NOT_FOUND();
	}
}

void Reader::ReadFile() {

	if (this->path.empty()) {
		auto message = "Must set the path before reading";
		cerr << message << endl;
		throw std::logic_error(message);
	}

	string line;
	ifstream file;

	this->IsFileExist();

	file.open(this->path);

    if(!file.is_open()) {
       cerr << "Error open" << endl;
       throw std::runtime_error("Failed to open file");
    }

    while(getline(file, line)) {
     	this->buffer.push_back(line);
    }

    file.close();
}

string Reader::GetLine(size_t index) {
	if (index >= this->buffer.size()) {
		auto message = "GetLine(index) Out of range";
		cerr << message << endl;
		throw std::range_error(message);
	}
	return this->buffer.at(index);
}

size_t Reader::GetLineCount() {
	if (this->buffer.empty()) {
		auto message = "Empty reader line buffer";
		cerr << message << endl;
		throw std::logic_error(message);
	}

	return this->buffer.size() - 1;
}

void Reader::ConcatBuffer(vector<string> other) {
	for (auto & line: other)
		this->buffer.push_back(line);
}

vector<string> Reader::GetBuffer() {
	return this->buffer;
}