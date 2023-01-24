#include "rendermath.h"

#include <math.h>

Point::Point() {
  this->x = 0;
  this->y = 0;
}

Point::Point(int newX, int newY) {
  this->x = newX;
  this->y = newY;
}

float Point::Distance(Point *other) {
  float result;

  result = pow(this->x - other->x, 2);
  result = pow(this->y - other->y, 2);
  result = sqrt(result);

  return result;
}

Point Point::operator*(float other) {
  Point point = *this;

  point.x *= other;
  point.y *= other;

  return point;
}

Point Point::operator/(float other) {
  Point point = *this;

  point.x /= other;
  point.y /= other;

  return point;
}

Point Point::operator+(Point other) {
  Point point = *this;

  point.x += other.x;
  point.y += other.y;

  return point;
}

Point Point::operator-(Point other) {
  Point point = *this;

  point.x -= other.x;
  point.y -= other.y;

  return point;
}
