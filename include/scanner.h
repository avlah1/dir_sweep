#include <map>
#include <algorithm>

#include "./main.h"
#include "./hasher.h"
#include "./rules.h"




bool scan_dir(const string& rootdir,
              uintmax_to_path_map* duplicates,
              path_vector* uniques);