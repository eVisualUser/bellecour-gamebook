#include "filesystem/reader.h"
#include "filesystem/ini.h"
#include "filesystem/toml.h"

#include <iostream>

using namespace std;
using namespace client_filesystem;

int main() {

	auto reader = Reader();
	reader.SetPath("demo.toml");
	reader.ReadFile();

	auto ini = Ini();
	ini.SetBuffer(reader.GetBuffer());

	for(auto & i: TomlParseArray(ini.ParseTable("choices").GetVar("choice_a").GetValue())) {
		cout << "- " << TomlParseString(i) << endl;
	}

	return 0;
}
