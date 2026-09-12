#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;

typedef std::vector<fs::path> path_vector;
typedef std::map<std::uintmax_t, std::vector<fs::path>> uintmax_to_path_map;
bool sweep(const string& path);