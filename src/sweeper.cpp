#include "../include/sweeper.h"
#include "../include/utils.h"

// Collects the files to be deleted from a root directory "path" by using other static helpers.
static bool scan_dir(const std::string& path,
                     path_vector& to_delete);

// Given a map of file size to path, extracts files from the map that are guaranteed to be unique (eg. the size maps only to one file) and keeps the 
// rest of the map intact.
static bool extract_uniques(uintmax_to_path_map& size_to_paths,
                            path_vector& uniques);

// Given a map of file size to path, fills a hashmap by hashing the bytes of every file in "size_to_paths" and placing them in "file_hashmap"                           
static bool fill_files_hashmap(uintmax_to_path_map& size_to_paths,
                             uintmax_to_path_map& file_hashmap);

// Given a map of hashed bytes to path, extracts any files that are unique and places them in the "uniques" vector, otherwise the path pointing to the newest 
// version(s) of the file are placed in "to_delete".
static bool process_files_hashmap(uintmax_to_path_map& file_hashmap,
                                  path_vector& uniques, 
                                  path_vector& to_delete);

bool sweep(const std::string& root_dir) {
  path_vector to_delete;
  if (!fs::exists(root_dir)) {
    // LOG
    std::cout << "path does not exist: " << root_dir << std::endl;
    return false;
  }
  scan_dir(root_dir, to_delete);
  for (const auto& p : to_delete) {
    std::error_code ec;
    bool success = fs::remove(p);
    // LOGS
    if (!success && ec) {
      std::cerr << "\t\tDeletion failed: " << ec.message() << " (Code " << ec.value() << "\n)";
    } else {
      std::cout << "\t\tDeletion successful: " << p << std::endl; 
    }
  }
  return true;
}

static bool scan_dir(const std::string& path,
                     path_vector& to_delete) {
  // map file size --> path
  uintmax_to_path_map size_to_paths;
  path_vector uniques;
  for (const auto& p : fs::recursive_directory_iterator(path)) {
    if (fs::is_regular_file(p)) {
      size_to_paths[fs::file_size(p)].push_back(p);
    }
  }
  
  // After the function call below, size_to_paths only contains mappings to possible duplicate files
  extract_uniques(size_to_paths, uniques);
  if (size_to_paths.size() > 0) {
    uintmax_to_path_map files_hashmap;
    fill_files_hashmap(size_to_paths, files_hashmap);
    // Extract uniques from the files_hashmap as well. The oldest duplicate for a file will be placed in "uniques" (we need to check if it is old),
    // while the newer versions will be placed in to_delete. Also, any file that was found not to have duplicates is also placed in "uniques".
    process_files_hashmap(files_hashmap, uniques, to_delete);
  }
  for (const auto& p : uniques) {
    if (file_time_expired(p)) {
      to_delete.push_back(p);
      // LOG
      std::cout << "\t\t" << path << "set for deletion, reason: older than " << THRESHOLD << "days" << std::endl;
    }
  }
  return true;
}

static bool extract_uniques(uintmax_to_path_map& size_to_paths,
                            path_vector& uniques) {
  uintmax_to_path_map::iterator it = size_to_paths.begin();
    while (it != size_to_paths.end()) {
      path_vector paths = it->second;
      if (paths.size() == 1) {
        // Impossible for there to be any duplicates in this vector so place the only path in "uniques" and then
        // remove the mapping from the map.
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
  // Iterate over what is left in the size_to_paths map, hashing every file that we come across and mapping it to its path name
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
  return true;
}

static bool process_files_hashmap(uintmax_to_path_map& files_hashmap,
                                  path_vector& uniques, 
                                  path_vector& to_delete) {
  for (const auto& e : files_hashmap) {
    path_vector paths = e.second;
    if (paths.size() == 1) {
      // Unique file!
      uniques.push_back(paths[0]);
    } else {
      sort(paths.begin(), paths.end(), &comp_file_times);
      // TODO: Make better, more idiomatic C++. For now this is the way i figured out how to do it.
      uniques.push_back(paths[0]);
      for (size_t i = 1; i < paths.size(); i++) {
        to_delete.push_back(paths[i]);
        // LOG
        std::cout << "\t\t" << paths[i] << "set for deletion, reason: duplicate " << std::endl;
      }
    }  
  } 
  return true;
}