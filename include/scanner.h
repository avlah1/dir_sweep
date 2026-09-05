#include <map>

#include "./main.h"

typedef std::map<std::uintmax_t, std::vector<fs::path>> uintmax_to_path_map;

bool scan_dir(const string& rootdir,
              const path_vector** poss_dups_ret,
              const path_vector** non_dups_ret);