#pragma once

#include "app_shared.h"

class GlassManager {
public:
  GlassManager()
      : glassess_{
            {.index = 0, .type = GlassStateType::GLASS_NONE},
            {.index = 1, .type = GlassStateType::GLASS_NONE},
            {.index = 2, .type = GlassStateType::GLASS_NONE},
            {.index = 3, .type = GlassStateType::GLASS_NONE},
            {.index = 4, .type = GlassStateType::GLASS_NONE},
            {.index = 5, .type = GlassStateType::GLASS_NONE},
        } {};

  void setStateType(uint8_t index, GlassStateType type);
  GlassState getGlass(uint8_t index) const;

private:
  GlassState glassess_[6];
};

extern GlassManager glassManager;