#pragma once

#include "reader.h"
#include "table.h"
#include "var.h"

#include <string>
#include <vector>

using namespace std;

#define INI_FMT_OPEN_TABLE '['
#define INI_FMT_CLOSE_TABLE ']'
#define INI_FMT_DEFINE_SIGN '='
#define INI_COMMENT_SIGN '#'

class Ini {
public:
  void SetBuffer(vector<string> newBuffer);
  void AddLine(string newLine);
  void GetTable(string name);
  Table ParseTable(string name);
  Var ParseVar(string line);
  vector<string> GetAllTables();

private:
  bool IsEmptyLine(string line); // Consider comment line as empty
  vector<string> buffer;
};

void trimmed(std::string &str);