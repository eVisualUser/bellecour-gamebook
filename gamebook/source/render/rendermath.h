#pragma once

class Point {
public:
  Point();
  Point(int newX, int newY);

public:
  float Distance(Point *other);

  Point operator*(float other);
  Point operator/(float other);
  Point operator+(Point other);
  Point operator-(Point other);

public:
  int x;
  int y;
};
