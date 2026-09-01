#include "../include/crawler.h"
namespace fs = std::filesystem;

typedef std::map<std::uintmax_t, std::vector<fs::path>> uintmax_to_path_map;

static void print_size_to_paths(const uintmax_to_path_map& size_to_paths);


bool crawl_file_tree(const string rootdir) {
  cout << "directory to crawl: " << rootdir << endl;
  uintmax_to_path_map size_to_paths;
  for (auto& p: fs::recursive_directory_iterator(rootdir)) {
    if (!fs::is_directory(p)) {
      std::uintmax_t size = fs::file_size(p);
      size_to_paths[size].push_back(p);
    }
  }
  print_size_to_paths(size_to_paths);
  return true;
}

static void print_size_to_paths(const uintmax_to_path_map& size_to_paths) {
  std::vector<fs::path>::iterator it;
  for (auto& e : size_to_paths) {
    cout << e.first << " bytes:" << endl;
    std::vector<fs::path> paths = e.second;
    for (it = paths.begin(); it < paths.end(); it++) {
      cout << "\t" << *it << ", ";
    }
    cout << endl;
  }
}
