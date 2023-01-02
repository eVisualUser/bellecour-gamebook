#pragma once

#include <string>

#include "Error.hpp"

using namespace std;

class GameBook {
public:
	Error Load(string folder);
private:
	string folder_path = "./";
};
