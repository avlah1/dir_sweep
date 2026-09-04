#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;

typedef std::vector<fs::path> path_vector;

bool sweep(const string& path);