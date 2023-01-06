#pragma once

#include <string>
#include <vector>

using namespace std;

#define TOML_FMT_OPEN_ARRAY '['
#define TOML_FMT_CLOSE_ARRAY ']'
#define TOML_FMT_SEPARATOR_ARRAY ','
#define TOML_FMT_COMMENT_LINE '#'

string TomlParseString(string input);
int TomlParseInt(string input);
vector<string> TomlParseArray(string input);
