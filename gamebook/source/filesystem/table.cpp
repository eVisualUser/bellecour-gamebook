#include "table.h"

#include <iostream>

void Table::AddVar(Var var) { this->content.push_back(var); }

Var Table::GetVar(string name) {
  for (auto &var : this->content) {
    if (var.GetKey() == name)
      return var;
  }

  return Var();
}

vector<Var> Table::GetAllVars() { return this->content; }
