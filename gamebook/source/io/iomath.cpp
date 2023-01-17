#include "iomath.h"

#include <iostream>

void clamp_int(int *base, int min, int max) {
  if (*base < min)
    *base = min;
  else if (*base > max)
    *base = max;
}
