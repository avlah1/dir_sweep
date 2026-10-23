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


bool comp_file_times(const fs::path a, const fs::path b) {

  fs::file_time_type a_time = fs::last_write_time(a);
  fs::file_time_type b_time = fs::last_write_time(b);
  
  // Favor the original (eg, the older file). This makes it so we don't have any files of the form file_name(copy n), where n is the n copy.
  return a_time < b_time;
}


bool file_time_expired(const fs::path& path) {
 
  ch::time_point file_time = fs::last_write_time(path);
  ch::time_point now = fs::__file_clock::now();
 
  return file_time < now - ch::days(THRESHOLD);
}

void print_vector(const path_vector& vec) {
  std::cout << "Vector:" << std::endl;
  for (const auto& e : vec) {
    std::cout << "\t" << e << std::endl;
  }
}

void print_map(const uintmax_to_path_map& map) {
  std::cout << "Map:" << std::endl;
  for (const auto& e : map) {
    path_vector vec = e.second;
    std::cout << "\tSize: " << e.first << std::endl;
    for (const auto& p : vec) {
      std::cout << "\t\t" << p << std::endl;
    }
  }
}