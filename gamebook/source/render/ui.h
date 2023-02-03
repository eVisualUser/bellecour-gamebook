#pragma once

#include <string>
#include <vector>

#include "../io/inputmanager.h"
#include "../logicstr/executor.h"
#include "../logicstr/stringformater.h"
#include "../logicstr/variablemanager.h"
#include "../pages/page.h"
#include "rendermath.h"

using namespace std;

class UI {
public:
  UI() {}
  UI(Point newSize);

public:
  void DrawLine(Point position, int lenght, char character);
  int DrawText(Point start, string text, VariableManager *variableManager,
               Executor *executor);
  void Zoom();
  void UnZoom(int minX, int minY);

public:
  void InitializeBuffer();
  void ResetBuffer();
  vector<string> GetFrame();
  void DrawButtons(Point position, InputManager *buttonManager,
                   char selectedChar, VariableManager *variableManager,
                   Executor *executor);
  Point size;

private:
  void UnsafeDrawTextAt(Point start, string text);
private:
  vector<string> buffer;
};
