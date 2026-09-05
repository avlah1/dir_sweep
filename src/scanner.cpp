#include "../include/scanner.h"

// *** STATIC HELPER DECLARATIONS *** //
static bool sort_paths(const uintmax_to_path_map& size_to_paths,
                       const path_vector** poss_dups_ret,
                       const path_vector** non_dups_ret);

static void print_size_to_paths(const uintmax_to_path_map& size_to_paths);

static void print_maps(const path_vector* poss_dups_ptr,
                       const path_vector* non_dups_ptr);
// ********************************** //

bool scan_dir(const string& rootdir,
              const path_vector** poss_dups_ret,
              const path_vector** non_dups_ret) {
  // Map size to paths
  uintmax_to_path_map size_to_paths;
  for (const auto& p : fs::recursive_directory_iterator(rootdir)) {
    if (fs::is_regular_file(p)) {
      size_to_paths[fs::file_size(p)].push_back(p);
    }
  }
  print_size_to_paths(size_to_paths);
  // Split the map entries into piles: one for paths to files that have the same size of at least one other file, and one for those that don't fit that category
  sort_paths(size_to_paths, poss_dups_ret, non_dups_ret);
  print_maps(*poss_dups_ret, *non_dups_ret);
  return true;
}

static bool sort_paths(const uintmax_to_path_map& size_to_paths,
                       const path_vector** poss_dups_ret,
                       const path_vector** non_dups_ret) {
  
  path_vector* same_size = new(path_vector); // Files that have the same amount of bytes as at least one other file
  path_vector* diff_size = new(path_vector); // Files that do not have the same amount of bytes as any other file
  for (const auto& e : size_to_paths) {
    path_vector paths = e.second;
    // If there is byte size that has more than one paths with the same number of bytes, add that vector of paths to the same_size vector
    if (paths.size() > 1) {
      path_vector::iterator it = same_size->begin();
      same_size->insert(it, paths.begin(), paths.end());
      // Otherwise, put it in the different size vector
    } else {
      diff_size->push_back(paths[0]);
    }
  }
  *poss_dups_ret = same_size;
  *non_dups_ret = diff_size;
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

static void print_maps(const path_vector* poss_dups_ptr,
                       const path_vector* non_dups_ptr) {
  path_vector poss_dups = *poss_dups_ptr;
  path_vector  non_dups = *non_dups_ptr;
  cout << "The following paths belong to files that may be duplicates of each other:" << endl;
  for (const auto& p : poss_dups) {
    cout << "\t" << p << endl;
  }
  cout << "The following paths belong to files that are unique:" << endl;
  for (const auto& p : non_dups) {
    cout << "\t" << p << endl;
  }
}




