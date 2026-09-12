#include <iostream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <chrono>

#define THRESHOLD 7;

namespace fs = std::filesystem;
namespace ch = std::chrono;

uint64_t fnvla_64(const void* data, size_t size);

bool comp_file_times(const fs::path a, const fs::path b);

bool file_time_expired(const fs::path& path);