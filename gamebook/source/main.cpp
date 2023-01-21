#include "core/core.h"
#include "debug/logger.h"
#include "splashframe/splashframe.h"

#include <conio.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

Core core = Core();
void AsyncGameLoop() { core.Update(); }
#endif

int main() {
  Logger::Log("Game Started");

  auto splashFrame = SplashFrame();
  splashFrame.Load("splash.txt");
  if (splashFrame.Render())
    _getch();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(AsyncGameLoop, 0, 1);
#else
  Core core = Core();
  core.RunGameLoop();
#endif
  return 0;
}
