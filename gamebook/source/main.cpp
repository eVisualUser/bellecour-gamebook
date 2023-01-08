#include <cstdio>
#include <iostream>

#include "filesystem/ini.h"

#include <string.h>
#include <stdio.h>

#include "system/file.h"

using namespace std;
int main() {

	cout << "Hello, World!" << endl;

	auto ini = client_filesystem::Ini();
	cout << ini.ParseVar("hello = 34").GetValue() << endl;

	return 0;
}
