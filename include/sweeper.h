#ifndef SWEEPER_H_
#define SWEEPER_H_

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace fs = std::filesystem;
namespace ch = std::chrono;

typedef std::vector<fs::path> path_vector;
typedef std::map<std::uintmax_t, std::vector<fs::path>> uintmax_to_path_map;

bool sweep(const std::string& path);

#endif // SWEEPER_H_