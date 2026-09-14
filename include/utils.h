#include <iostream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <chrono>
#include <vector>
#include <map>

#define THRESHOLD 7

namespace fs = std::filesystem;
namespace ch = std::chrono;

typedef std::vector<fs::path> path_vector;
typedef std::map<std::uintmax_t, std::vector<fs::path>> uintmax_to_path_map;

// Simple hashing algorithm
uint64_t fnvla_64(const void* data, size_t size);

// Comparator function that returns true if file with path a is older than file with path b.
bool comp_file_times(const fs::path a, const fs::path b);

// Returns true if file with path "path" is older than the threshold defined above.
bool file_time_expired(const fs::path& path);

void print_vector(const path_vector& vec);

void print_map(const uintmax_to_path_map& map);