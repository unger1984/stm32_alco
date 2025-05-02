#include "utils.h"

int compareStrings(const char *a, const char *b) {
  while (*a && *b && *a == *b) {
    a++;
    b++;
  }

  if (*a == *b)
    return 0;
  return (*a < *b) ? -1 : 1;
};

uint8_t isStringEqual(const char *a, const char *b) {
  return compareStrings(a, b) == 0 ? 1 : 0;
}