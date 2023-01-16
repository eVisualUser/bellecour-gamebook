#pragma once

#include <string>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <iostream>
#endif

void PrintError(std::string message);
