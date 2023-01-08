#include "file.h"

#include <iostream>

File::File(string path) {
	this->SetPath(path);
}

void File::SetPath(string path) {
	this->path = path;
}

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

void File::PathExist() {
	EM_JS(bool, path_exist, (string path), {
  		var path = require('path');
  		return path.existsSync(path);
	});

	return path_exist(this->path);
}

void File::Load() {
	if (!this->PathExist()) {
		cerr << ERROR_MSG_FILE_NOT_FOUND << this->path << endl;
		throw std::runtime_error("File not found");
	}

	this->loaded = true;

	
}
#else // Native

#include <fstream>

bool File::PathExist() {

	ifstream file;
	file.open(this->path);

	if (file) {
		file.close();
		return true;
	}

	file.close();
	return false;
}

void File::Load() {
	if (!this->PathExist()) {
		cerr << ERROR_MSG_FILE_NOT_FOUND << ": " << this->path << endl;
		throw std::runtime_error(ERROR_MSG_FILE_NOT_FOUND);
	}

	this->loaded = true;

	if (this->path.empty()) {
		cerr << ERROR_MSG_FILE_PATH_MISSING << endl;
		throw std::logic_error(ERROR_MSG_FILE_PATH_MISSING);
	}

	string line;
	ifstream file;

	if (this->PathExist()) {
		file.open(this->path);

	    if(!file.is_open()) {
	       cerr << ERROR_MSG_FAILED_TO_OPEN << ": " << this->path << endl;
	       throw std::runtime_error(ERROR_MSG_FAILED_TO_OPEN);
	    }

	    while(getline(file, line)) {
	     	this->buffer.push_back(line);
	    }

	    file.close();
	}
}
#endif
