#pragma once

#include <string>

using namespace std;

#define RETURN_ERROR_DESACTIVATED \
auto error = Error(); \
error.Desactivate(); \
return error;

class Error {
public:
	void SetTitle(string newTitle);
	void SetMessage(string newDescription);
	void Show();
	void Active();
	void Desactivate();
private:
	string title = "Error title";
	string message = "Error description";
	bool active = false;
};
