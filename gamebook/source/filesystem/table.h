#pragma once

#include "var.h"

#include <string>
#include <vector>

using namespace std;

namespace client_filesystem {
class Table {
public:
  void AddVar(Var var);
  vector<Var> GetAllVars();
  Var GetVar(string name);
  vector<Var> content;
};
} // namespace client_filesystem
