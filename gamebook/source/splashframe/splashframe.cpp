#include "splashframe.h"

#include "../filesystem/reader.h"
#include "../debug/logger.h"

#include <stdexcept>
#include <iostream>

void SplashFrame::Load(string file) {
#ifdef __EMSCRIPTEN__
#else
    auto reader = Reader();
    try {
      reader.SetPath(file);
      reader.ReadFile();
    } catch(runtime_error error) {
        Logger::LogWarning("No splash ART");
        return;
    }

    this->buffer = reader.GetBuffer();
#endif
}

bool SplashFrame::Render() {
#ifdef __EMSCRIPTEN__
#else

    if (this->buffer.empty())
      return false;

    system("cls");

    for (auto & line: this->buffer) {
      cout << line << endl;
    }
    return true;
#endif
  return false;
}
