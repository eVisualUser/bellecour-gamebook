#include "message.h"

void PrintError(std::string message) {
	#ifdef _WIN32
		MessageBox(NULL, LPCSTR(message.c_str()), LPCSTR("ERROR"), MB_OK);
	#else
		std::cerr << "[ERROR] => " << message << std::endl;
	#endif
}
