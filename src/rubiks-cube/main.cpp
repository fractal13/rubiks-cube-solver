#include "cglCubeSolver.h"
#include <iostream>
#include <unistd.h>

int main(int /*argc*/, char **/*argv*/) {
  bool show_prompt = isatty(STDIN_FILENO);
  cgl::cube::process_input_stream( std::cin, show_prompt );
  return 0;
}
