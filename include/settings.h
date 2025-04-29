#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  U8,
  U32,
} SettingsSize_t;

typedef struct GlassAngles {
  uint8_t a1, a2, a3, a4, a5, a6;
} GlassAngles_t;

typedef struct GlassDoses {
  uint8_t d1, d2, d3, d4, d5, d6;
} GlassDoses_t;

typedef struct Settings {
  uint32_t calibration; // время налива стопки
  GlassAngles_t angles; // углы серво
  GlassDoses_t doses;   // дозы по стопкам
} Settings_t;

extern Settings_t currentSettings;

#ifdef __cplusplus
}
#endif

#endif /* _SETTINGS_H */