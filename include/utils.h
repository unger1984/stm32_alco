#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief  Сравниваем стоки
/// @param a
/// @param b
/// @return
int compareStrings(const char *a, const char *b);

uint8_t isStringEqueal(const char *a, const char *b);

uint32_t utf8_strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_H */