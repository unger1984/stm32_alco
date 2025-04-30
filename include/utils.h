#pragma once

#include <stdint.h>

#define millis() ((osKernelGetTickCount() * 1000U) / osKernelGetTickFreq())

/// @brief  Сравниваем стоки
/// @param a
/// @param b
/// @return
int compareStrings(const char *a, const char *b);

uint8_t isStringEqual(const char *a, const char *b);

uint32_t utf8_strlen(const char *str);

template <typename T> T clamp(T val, T minVal, T maxVal) {
  return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}

template <typename T> constexpr const T &max(const T &a, const T &b) {
  return (a < b) ? b : a;
}