#include "noise.h"

int RandomRange(int min, int max) {
  srand(time(NULL));
  return rand() % max - min + 1;
}

void ApplyTextNoise(vector<string> *source, vector<char> chars, float ratio) {
  if (ratio > 0) {
    for (int i = 0; i < (*source).size()-1; i++) {
      int count = (*source)[i].length() * ratio;
      for (int j = 0; j < (*source)[i].length(); j++) {
        int rnd = RandomRange(0, count);
        int rnd_char = RandomRange(1, chars.size()-1);
        if (j < count && j % 2 == 1) {
          if ((*source)[i][j] != ' ')
            (*source)[i][j] = chars[rnd_char];
        }
      }
    }
  }
}
