#pragma once

#include <string>
#include <vector>

using namespace std;

/// Simple object to read "splash.txt" and print it to the screen.
class SplashFrame {
public:
    void Load(string file);
    bool Render();
private:
  vector<string> buffer;
};
