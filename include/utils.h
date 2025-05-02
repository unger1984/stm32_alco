#pragma once

#include <stdint.h>

#define millis() ((osKernelGetTickCount() * 1000U) / osKernelGetTickFreq())

/// @brief  Сравниваем стоки
/// @param a
/// @param b
/// @return
int compareStrings(const char *a, const char *b);

uint8_t isStringEqual(const char *a, const char *b);

/// @brief Ограничивает число val минимальным и максимальным значением
/// @tparam T
/// @param val
/// @param minVal
/// @param maxVal
/// @return
template <typename T> T clamp(T val, T minVal, T maxVal) {
  return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}

/// @brief Возвращает максимальное значение из двух чисел
/// @tparam T
/// @param a
/// @param maxVal
/// @return
template <typename T> constexpr const T &max(const T &a, const T &b) {
  return (a < b) ? b : a;
}

/// @brief Линейно преобразует значение из одного диапазона в другой
/// @tparam T
/// @param val
/// @param in_min
/// @param in_max
/// @param out_min
/// @param out_max
/// @return
template <typename T> T map(T val, T in_min, T in_max, T out_min, T out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T, typename R>
R map(T val, T in_min, T in_max, R out_min, R out_max) {
  return static_cast<R>(
      (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}