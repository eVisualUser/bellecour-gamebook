#include "core/core.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include "error/message.h"

Core core = Core();

void AsyncGameLoop() {
	core.Update();
}
#endif

int main() {
	//PrintError("Test Error");
	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(AsyncGameLoop, 0, 1);
	#else
		Core core = Core();
		core.RunGameLoop();
	#endif
	return 0;
}
