#include "../include/main.h"
#include "../include/scanner.h"


int main (int argc, char** argv) {
  if (argc > 1) {
    sweep(argv[1]);
  }
  return 0;
}

bool sweep(const string& path) {
  const path_vector* poss_dups = new (path_vector);
  const path_vector* non_dups = new (path_vector);
  scan_dir(path, &poss_dups, &non_dups);
  return true;
}

