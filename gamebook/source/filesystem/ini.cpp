#include "ini.h"

#include <iostream>

void Ini::SetBuffer(vector<string> newBuffer) { this->buffer = newBuffer; }

void Ini::AddLine(string newLine) { this->buffer.push_back(newLine); }

bool Ini::IsEmptyLine(string line) {
  for (auto &i : line) {
    if (i == INI_COMMENT_SIGN) {
      break;
    } else if (i != ' ') {
      return false;
    }
  }
  return true;
}

Table Ini::ParseTable(string name) {
  auto table = Table();

  bool inTable = false;
  for (auto &line : this->buffer) {
    if (line[0] == INI_FMT_OPEN_TABLE) {
      string buffer;
      for (auto &i : line) {
        if (i != INI_FMT_CLOSE_TABLE) {
          if (i != INI_FMT_OPEN_TABLE)
            buffer.push_back(i);
        } else if (buffer == name) {
          inTable = true;
          break;
        } else {
          inTable = false;
          break;
        }
      }

    } else if (inTable && !IsEmptyLine(line)) {
      table.AddVar(this->ParseVar(line));
    }
  }
  return table;
}

void trimmed(std::string &str) { // From stack-overflow
  int begI = 0, endI = str.length();
  if (endI == begI)
    return;
  std::string::iterator beg = str.begin();
  std::string::iterator end = str.end();
  end--;
  while (isspace(*beg) || isspace(*end)) {
    if (isspace(*beg)) {
      beg++;
      begI++;
    }
    if (isspace(*end)) {
      end--;
      endI--;
    }
  }
  str = str.substr(begI, (endI - begI));
}

Var Ini::ParseVar(string line) {
  auto var = Var();

  bool isKey = true;
  bool varEqualRemoved = false;

  string key;
  string value;

  for (char &i : line) {
    if (i != ' ') {
      if (i == INI_COMMENT_SIGN)
        break;

      if (i == INI_FMT_DEFINE_SIGN) {
        isKey = false;
      }

      if (isKey)
        key.push_back(i);
      else if (!varEqualRemoved && i == INI_FMT_DEFINE_SIGN)
        varEqualRemoved = true;
      else
        value.push_back(i);

    } else {
      value.push_back(i);
    }
  }

  trimmed(value);
  var.Set(key, value);

  return var;
}

vector<string> Ini::GetAllTables() {

  vector<string> result;

  for (auto &line : this->buffer) {
    if (line[0] == INI_FMT_OPEN_TABLE) {
      string buffer;
      for (auto &i : line) {
        if (i == INI_FMT_CLOSE_TABLE)
          break;
        else if (i != INI_FMT_OPEN_TABLE)
          buffer.push_back(i);
      }
      result.push_back(buffer);
    }
  }

  return result;
}