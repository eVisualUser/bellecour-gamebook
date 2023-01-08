#pragma once

#include <string>
#include <vector>

using namespace std;

#define ERROR_MSG_FILE_NOT_FOUND "File not found"
#define ERROR_MSG_FILE_PATH_MISSING "Missing file path"
#define ERROR_MSG_FAILED_TO_OPEN "Failed to open file"

class File {
public:
	File(string path);
	void SetPath(string path);
	bool PathExist();
	void Load();
	void Save();
	bool IsLoaded() { return this->loaded; }
	vector<string> GetBuffer();
private:
	bool loaded = false;
	string path;
	vector<string> buffer;
};
