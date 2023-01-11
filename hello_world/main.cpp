#include <conio.h>
#include <iostream>

using namespace std;

int main() {

	cout << "Hello, World!" << endl;

	cout << "Press ESCAPE to exit ...";

	while(_getch() != 27) {}

	return 0;
}
