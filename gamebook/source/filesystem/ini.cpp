#include "ini.h"

using namespace client_filesystem;

void Ini::SetBuffer(vector<string> newBuffer) {
	this->buffer = newBuffer;
}

void Ini::AddLine(string newLine) {
	this->buffer.push_back(newLine);
}

bool Ini::IsEmptyLine(string line) {
	for(auto & i: line) {
		if (i == INI_COMMENT_SIGN) {
			break;
		}
		if (i != ' ') {
			return false;
		}
	}
	return true;
}

Table Ini::ParseTable(string name) {
	auto table = Table();

	bool inTable = false;
	for(auto & line: this->buffer) {

		if (line[0] == INI_FMT_OPEN_TABLE) {
			string buffer;
			for (auto & i: line) {
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

Var Ini::ParseVar(string line) {
	auto var = Var();

	bool isKey = true;

	string key;
	string value;

	for(char & i: line) {
		if (i != ' ') {
		if (i == INI_COMMENT_SIGN)
			break;

		if (i == INI_FMT_DEFINE_SIGN) {
			isKey = false;
		}

		if (isKey)
			key.push_back(i);
		else if (i != INI_FMT_DEFINE_SIGN)
			value.push_back(i);
		}	
	}

	var.Set(key, value);

	return var;
}
