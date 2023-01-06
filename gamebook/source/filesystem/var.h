#pragma once

#include <string>

using namespace std;

namespace client_filesystem {
	class Var {
	public:
		string GetKey();
		string GetValue();
		void Set(string key, string value);
	private:
		string key;
		string value;
	};
}
