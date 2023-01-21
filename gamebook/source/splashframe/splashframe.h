#pragma once

#include <string>
#include <vector>

using namespace std;

class SplashFrame {
public:
    void Load(string file);
    bool Render();
private:
  vector<string> buffer;
};
