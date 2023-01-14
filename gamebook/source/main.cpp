#include "core/core.h"

#ifdef __EMSCRIPTEN__

Core core = Core();

void AsyncGameLoop() {
	core.Update();
}
#endif

int main() {
	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(AsyncGameLoop, 0, 1);
	#else
		Core core = Core();
		core.RunGameLoop();
	#endif
	return 0;
}
