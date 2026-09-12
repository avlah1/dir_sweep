#include "../include/scanner.h"

// *** STATIC HELPER DECLARATIONS *** //
static bool sort_paths(uintmax_to_path_map& size_to_paths,
                       path_vector& uniques);
                       

static void print_size_to_paths(const uintmax_to_path_map& size_to_paths);

static void print_vec(const path_vector& vec);

static bool handle_dups(uintmax_to_path_map& size_to_paths,
                     path_vector& uniques,
                     path_vector& to_delete);
// ********************************** //

bool scan_dir(const string& rootdir,
              uintmax_to_path_map* duplicates_ptr,
              path_vector* uniques_ptr) {
  uintmax_to_path_map size_to_paths;
  path_vector uniques, to_delete;
  for (const auto& p : fs::recursive_directory_iterator(rootdir)) {
    if (fs::is_regular_file(p)) {
      size_to_paths[fs::file_size(p)].push_back(p);
    }
  }
  sort_paths(size_to_paths, uniques);
  // size_to_paths now contains only pairs who have vectors of size 2 or more
  // uniques now contains the pairs removed from the map

  handle_dups(size_to_paths, uniques, to_delete);
  for (const auto& p : uniques) {
    if (is_week_old(p)) {
      to_delete.push_back(p);
    }
  }
  return true;
}

static bool handle_dups(uintmax_to_path_map& size_to_paths,
                     path_vector& uniques,
                     path_vector& to_delete) {
  uintmax_to_path_map paths_hashmap;
  uintmax_to_path_map::iterator it = size_to_paths.begin();
  while (it != size_to_paths.end()) {
    path_vector paths = it->second;
    for (const auto& p : paths) {
      std::ifstream file(p, std::ios::binary);
      std::vector<char> bytes(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>()));
      uint64_t hash = fnvla_64(bytes.data(), bytes.size());
      paths_hashmap[hash].push_back(p);
    }
    it++;
  }
  // need to iterate over the paths hashmap. for every pair that has vec of size 1, push to uniques
  // otherwise, we need to sort the vector by last modify date
  for (const auto& e : paths_hashmap) {
    path_vector paths = e.second;
    if (paths.size() == 1) {
      uniques.push_back(paths[0]);
    } else {
      sort(paths.begin(), paths.end(), &is_older_file);
      // now the paths are sorted, with the first path being the one we will keep
      // TODO: Make better, more idiomatic C++. For now this is the way i figured out how to do it.
      uniques.push_back(paths[0]);
      for (size_t i = 1; i < paths.size(); i++) {
        to_delete.push_back(paths[i]);
      }
       print_vec(uniques);
       print_vec(to_delete);
    }  
  } 
  return true;
}
static bool sort_paths(uintmax_to_path_map& size_to_paths,
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


// *** HELPERS FOR DEBUGGING ETC *** //
static void print_size_to_paths(const uintmax_to_path_map& size_to_paths) {
  path_vector::iterator it;
  for (auto& e : size_to_paths) {
    cout << e.first << " bytes:" << endl;
    path_vector paths = e.second;
    for (it = paths.begin(); it < paths.end(); it++) {
      cout << "\t" << *it << ", ";
    }
    cout << endl;
  }
}

static void print_vec(const path_vector& vec) {
  cout << "Path vector:" << endl;
  for (const auto& p : vec) {
    cout << "\t" << p << endl;
  }
}




