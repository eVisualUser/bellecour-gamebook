#pragma once

#include <vector>
#include <string>
#include <random>
#include <time.h>

using namespace std;

int RandomRange(int min, int max);

void ApplyTextNoise(vector<string> *source, vector<char> chars, float ratio);