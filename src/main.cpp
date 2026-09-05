#include "../include/main.h"
#include "../include/scanner.h"
#include "../include/hasher.h"


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

  std::map<uint64_t, std::vector<fs::path>> file_hashmap;
  // What we have now:
  for (const auto& p : *poss_dups) {
    std::ifstream file(p, std::ios::binary);
    std::vector<char> bytes(
         (std::istreambuf_iterator<char>(file)),
         (std::istreambuf_iterator<char>()));
    uint64_t hash = fnvla_64(bytes.data(), bytes.size());
    file_hashmap[hash].push_back(p);
  }
  // call rules is duplicate

  // A vector that stores a bunch of paths that are not duplicates (these need to be passed to rules)
    
  return true;
}

