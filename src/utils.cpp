#include "../include/utils.h"

uint64_t fnvla_64(const void* data, size_t size) {
  const unsigned char* bytes = static_cast<const unsigned char*>(data);
  uint64_t hash = 14695981039346656037ull;

  for (size_t i = 0; i < size; i++) {
    hash ^= bytes[i];
    hash *= 1099511628211ull;
  }

  return hash;
}


bool is_older_file(std::filesystem::path a, std::filesystem::path b) {
  // so if a is older than b, return true

  std::filesystem::file_time_type a_time = std::filesystem::last_write_time(a);
  std::filesystem::file_time_type b_time = std::filesystem::last_write_time(b);

  auto a_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(a_time.time_since_epoch());
  auto b_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(b_time.time_since_epoch());
  
  // Favor the original (eg, the older file). This makes it so we don't have any files of the form file_name(copy n), where n is the nth copy
  return a_time_ms.count() < b_time_ms.count();
}