#include "../include/sweeper.h"

int main (int argc, char** argv) {
  if (argc > 1) {
    if (sweep(argv[1])) {
      return 0;
    } else {
      return 1;
    }
  }
  return 0;
}


