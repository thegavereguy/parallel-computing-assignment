#include <catch2/catch_session.hpp>
#include <iostream>

int main(int argc, char** argv) {
  Catch::Session session;

  int height = 0;

  using namespace Catch::Clara;
  auto cli = session.cli() | Opt(height, "height")["--height"]("how high?");

  session.cli(cli);

  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) return returnCode;

  return session.run();
}
