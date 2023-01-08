#pragma once

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <string>

	std::string get_input() {
	  // Create a string to hold the input
	  std::string input;

	  // Use the EM_ASM macro to execute JavaScript code that prompts the user for input
	  // and stores the input in a variable
	  EM_ASM({
	    var input = prompt('Enter something:', 'Write here');
	    setValue($0, input);
	  }, &input);

	  // Return the input
	  return input;
	}
#endif
