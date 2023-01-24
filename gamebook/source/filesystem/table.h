#pragma once

#include "var.h"

#include <string>
#include <vector>

using namespace std;

class Table {
public:
  void AddVar(Var var);
  vector<Var> GetAllVars();
  Var GetVar(string name);
  vector<Var> content;
};
