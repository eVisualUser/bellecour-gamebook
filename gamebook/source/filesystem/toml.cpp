#include "toml.h"

string TomlParseString(string input) {
	string result;
	int j = 0;
	for (auto & i: input) {
		if (j == 0 && i == ' ')
			continue;
		if (i != '\"') 
			result.push_back(i);
		j++;
	}
	return result;
}

int TomlParseInt(string input) {
	string result;

	for(auto & i: input) {
		if (i != ' ' && i != '\n')
			result.push_back(i);
	}
	
	return stoi(result);
}

vector<string> TomlParseArray(string input) {
	vector<string> result;

	string buffer;
	for(auto & i: input) {
		if (i == TOML_FMT_COMMENT_LINE)
			break;

		if (i != TOML_FMT_OPEN_ARRAY && i != TOML_FMT_CLOSE_ARRAY && i != TOML_FMT_SEPARATOR_ARRAY) {
			buffer.push_back(i);
		}
		if (i == TOML_FMT_SEPARATOR_ARRAY) {
			result.push_back(buffer);
			buffer = "";
		}
	}
	result.push_back(buffer);

	return result;
}
