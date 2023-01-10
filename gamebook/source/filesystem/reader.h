#pragma once

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Reader {
public:
	void ReadFile();

	void ConcatBuffer(vector<string> other);
	vector<string> GetBuffer();

	void SetPath(string newPath);

	string GetLine(size_t index);
	size_t GetLineCount();
private:
	bool IsFileExist();
private:
	string path;
	vector<string> buffer;
};
