#include "../include/main.h"
#include "../include/sweeper.h"

int main (int argc, char** argv) {
  if (argc > 1) {
    sweep(argv[1]);
  }
  return 0;
}


