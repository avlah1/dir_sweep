#include "../include/sweeper.h"
#include "../include/utils.h"

static bool scan_dir(const std::string& path,
                     path_vector& to_delete);

static bool extract_uniques(uintmax_to_path_map& size_to_paths,
                            path_vector& uniques);

static bool fill_files_hashmap(uintmax_to_path_map& size_to_paths,
                             uintmax_to_path_map& file_hashmap);

static bool process_files_hashmap(uintmax_to_path_map& file_hashmap,
                                  path_vector& uniques, 
                                  path_vector& to_delete);

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
  uintmax_to_path_map files_hashmap;
  fill_files_hashmap(size_to_paths, files_hashmap);
  process_files_hashmap(files_hashmap, uniques, to_delete);
  for (const auto& p : uniques) {
    if (file_time_expired(path)) {
      to_delete.push_back(p);
    }
  }
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

static bool fill_files_hashmap(uintmax_to_path_map& size_to_paths,
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

static bool process_files_hashmap(uintmax_to_path_map& files_hashmap,
                                  path_vector& uniques, 
                                  path_vector& to_delete) {
  for (const auto& e : files_hashmap) {
    path_vector paths = e.second;
    if (paths.size() == 1) {
      uniques.push_back(paths[0]);
    } else {
      sort(paths.begin(), paths.end(), &comp_file_times);
      // now the paths are sorted, with the first path being the one we will keep
      // TODO: Make better, more idiomatic C++. For now this is the way i figured out how to do it.
      uniques.push_back(paths[0]);
      for (size_t i = 1; i < paths.size(); i++) {
        to_delete.push_back(paths[i]);
      }
    }  
  } 
  return true;
}