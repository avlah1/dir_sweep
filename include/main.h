#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::string;

typedef std::vector<fs::path> path_vector;

bool sweep(const string& path);