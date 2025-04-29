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

uint32_t utf8_strlen(const char *str) {
  uint32_t len = 0;
  const unsigned char *s = (const unsigned char *)str;

  while (*s) {
    if ((*s & 0x80) == 0x00) {
      // 1-byte (ASCII)
      s += 1;
    } else if ((*s & 0xE0) == 0xC0 && (s[1] & 0xC0) == 0x80) {
      // 2-byte
      s += 2;
    } else if ((*s & 0xF0) == 0xE0 && (s[1] & 0xC0) == 0x80 &&
               (s[2] & 0xC0) == 0x80) {
      // 3-byte
      s += 3;
    } else if ((*s & 0xF8) == 0xF0 && (s[1] & 0xC0) == 0x80 &&
               (s[2] & 0xC0) == 0x80 && (s[3] & 0xC0) == 0x80) {
      // 4-byte
      s += 4;
    } else {
      // некорректный байт — пропускаем 1
      s += 1;
    }
    len++;
  }

  return len;
}