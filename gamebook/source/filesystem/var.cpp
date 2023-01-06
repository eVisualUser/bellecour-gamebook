#include "var.h"



using namespace client_filesystem;

string Var::GetKey() {
	return this->key;
}

string Var::GetValue() {
	return this->value;
}

void Var::Set(string newKey, string newValue) {
	this->key = newKey;
	this->value = newValue;
}
