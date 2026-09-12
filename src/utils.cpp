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
  // so if a is older than b, return true

  fs::file_time_type a_time = fs::last_write_time(a);
  fs::file_time_type b_time = fs::last_write_time(b);

  auto a_time_ms = ch::duration_cast<ch::milliseconds>(a_time.time_since_epoch());
  auto b_time_ms = ch::duration_cast<ch::milliseconds>(b_time.time_since_epoch());
  
  // Favor the original (eg, the older file). This makes it so we don't have any files of the form file_name(copy n), where n is the nth copy
  return a_time_ms.count() < b_time_ms.count();
}

bool file_time_expired(const fs::path& path) {
  ch::time_point fc_now = fs::__file_clock::now();
  ch::milliseconds fc_now_ms = ch::duration_cast<ch::milliseconds>(fc_now.time_since_epoch());
  
  ch::time_point file_origin = fs::last_write_time(path);
  ch::milliseconds file_origin_ms = ch::duration_cast<ch::milliseconds>(file_origin.time_since_epoch());
 
  fc_now_ms *= -1;
  file_origin_ms *= -1;
  ch::milliseconds diff = file_origin_ms - fc_now_ms;

  diff /= 1000; // gives seconds
  diff /= 60; // gives minutes
  diff /= 60; // gives hours
  diff /= 24; // gives days

  return diff.count() >= THRESHOLD;
}