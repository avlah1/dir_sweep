#include "../include/hasher.h"

uint64_t fnvla_64(const void* data, size_t size) {
  const unsigned char* bytes = static_cast<const unsigned char*>(data);
  uint64_t hash = 14695981039346656037ull;

  for (size_t i = 0; i < size; i++) {
    hash ^= bytes[i];
    hash *= 1099511628211ull;
  }

  return hash;
}