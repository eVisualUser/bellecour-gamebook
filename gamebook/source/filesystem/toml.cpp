#include "toml.h"

string TomlParseString(string input) {
	string result;
	for (int i = 1; i < input.size()-2; i++)
		result.push_back(input[i]);
	return result;
}

int TomlParseInt(string input) {
	return stoi(input);
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
