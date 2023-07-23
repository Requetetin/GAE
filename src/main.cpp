#include <iostream>
#include <print.h>
#include <Game.h>

int main(int argc, char* args[]) {
  Game pong = Game("pong", 1020, 600);

  pong.run();
  return 0;
}
