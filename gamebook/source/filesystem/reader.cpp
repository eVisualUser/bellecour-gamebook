#include "reader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

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
  if (!this->IsFileExist()) {
    stringstream message;
    message << "File not found: " << newPath;
    throw runtime_error(message.str());
  }
}

void Reader::ReadFile() {

  if (this->path.empty()) {
    throw runtime_error("Must set the path before reading");
  }

  string line;
  ifstream file;

  if (!this->IsFileExist()) {
    throw "File not found";
  }

  file.open(this->path);

  if (!file.is_open()) {
    throw runtime_error("Failed to open file");
  }

  while (getline(file, line)) {
    this->buffer.push_back(line);
  }

  file.close();
}

string Reader::GetLine(size_t index) {
  if (index >= this->buffer.size()) {
    auto message = "GetLine(index) Out of range";
    throw runtime_error(message);
  }
  return this->buffer.at(index);
}

size_t Reader::GetLineCount() {
  if (this->buffer.empty()) {
    throw runtime_error("Empty reader line buffer");
  }

  return this->buffer.size() - 1;
}

void Reader::ConcatBuffer(vector<string> other) {
  for (auto &line : other)
    this->buffer.push_back(line);
}

vector<string> Reader::GetBuffer() { return this->buffer; }
