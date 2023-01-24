#pragma once

#include <random>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

int RandomRange(int min, int max);

void ApplyTextNoise(vector<string> *source, vector<char> chars, float ratio);
