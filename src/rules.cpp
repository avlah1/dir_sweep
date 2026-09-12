#include "../include/rules.h"

bool is_week_old(const fs::path& path) {

  std::chrono::time_point fc_now = std::filesystem::__file_clock::now();
  std::chrono::milliseconds fc_now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fc_now.time_since_epoch());
  
  std::chrono::time_point file_origin = fs::last_write_time(path);
  std::chrono::milliseconds file_origin_ms = std::chrono::duration_cast<std::chrono::milliseconds>(file_origin.time_since_epoch());
 
  fc_now_ms *= -1;
  file_origin_ms *= -1;
  std::chrono::milliseconds diff = file_origin_ms - fc_now_ms;

  diff /= 1000; // gives seconds
  diff /= 60; // gives minutes
  diff /= 60; // gives hours
  diff /= 24; // gives days

  return diff.count() >= 7;
}