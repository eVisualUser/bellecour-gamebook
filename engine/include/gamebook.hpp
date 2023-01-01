#pragma once

#include <string>

#include "error.hpp"

using namespace std;

class GameBook {
public:
	Error Load(string folder);
private:
	string folder_path = "./";
};
