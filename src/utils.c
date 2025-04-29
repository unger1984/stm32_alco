#include "utils.h"

int compareStrings(const char *a, const char *b) {
  while (*a && *b && *a == *b) {
    a++;
    b++;
  }
  return *a - *b;
};

uint8_t isStringEqueal(const char *a, const char *b) {
  return compareStrings(a, b) ? 0 : 1;
}

uint32_t utf8_strlen(const char *str) {
  uint32_t len = 0;
  while (*str) {
    if ((*str & 0xC0) != 0x80)
      len++; // Считаем только стартовые байты символов
    str++;
  }
  return len;
}