#include <iostream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <chrono>

uint64_t fnvla_64(const void* data, size_t size);

bool is_older_file(std::filesystem::path a, std::filesystem::path b);