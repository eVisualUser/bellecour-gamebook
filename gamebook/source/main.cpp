#include <cstdio>
#include <iostream>

#include "filesystem/ini.h"

#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
	#include "console/input.h"
#endif

using namespace std;
int main() {

	cout << "Hello, World!" << endl;

	auto ini = client_filesystem::Ini();
	cout << ini.ParseVar("hello = 34").GetValue() << endl;

	#ifdef __EMSCRIPTEN__
		cout << get_input();
	#endif

	return 0;
}
