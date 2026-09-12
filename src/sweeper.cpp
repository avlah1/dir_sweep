#include "../include/sweeper.h"

static bool scan_dir(const std::string& path,
                     path_vector& to_delete);

static bool extract_uniques(uintmax_to_path_map& size_to_paths,
                            path_vector& uniques);

bool sweep(const std::string& root_dir) {
  path_vector to_delete;
  scan_dir(root_dir, to_delete);


  
  return true;
}

static bool scan_dir(const std::string& path,
                     path_vector& to_delete) {
  uintmax_to_path_map size_to_paths;
  path_vector uniques;
  for (const auto& p : fs::recursive_directory_iterator(path)) {
    if (fs::is_regular_file(p)) {
      size_to_paths[fs::file_size(p)].push_back(p);
    }
  }
  extract_uniques(size_to_paths, uniques);

}

static bool extract_uniques(uintmax_to_path_map& size_to_paths,
                            path_vector& uniques) {
  uintmax_to_path_map::iterator it = size_to_paths.begin();
    while (it != size_to_paths.end()) {
      path_vector paths = it->second;
      if (paths.size() <= 1) {
        uniques.push_back(paths[0]);
        it = size_to_paths.erase(it);
      } else {
        it++;
      }
    }
  return true;
}

static bool fill_file_hashmap(uintmax_to_path_map& size_to_paths,
                             uintmax_to_path_map& file_hashmap) {
  uintmax_to_path_map::iterator it = size_to_paths.begin();
  while (it != size_to_paths.end()) {
    path_vector paths = it->second;
    for (const auto& p : paths) {
      std::ifstream file(p, std::ios::binary);
      std::vector<char> bytes(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>()));
      uint64_t hash = fnvla_64(bytes.data(), bytes.size());
      file_hashmap[hash].push_back(p);
    }
    it++;
  }
}