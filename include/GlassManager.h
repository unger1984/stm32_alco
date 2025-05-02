#pragma once

#include "app_shared.h"

class GlassManager {
public:
  GlassManager()
      : glassess_{
            {.index = 0, .poured = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 1, .poured = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 2, .poured = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 3, .poured = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 4, .poured = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 5, .poured = 0, .type = GlassStateType::GLASS_NONE},
        } {};

  void setStateType(uint8_t index, GlassStateType type);
  void setStatePoured(uint8_t index, uint8_t value);
  GlassState getGlass(uint8_t index) const;

private:
  GlassState glassess_[6];
};

extern GlassManager glassManager;