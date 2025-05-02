#pragma once

#include "AppSettings.h"
#include "Glass.h"

class GlassManager {
public:
  GlassManager()
      : glassess_{
            Glass(0), Glass(1), Glass(2), Glass(3), Glass(4), Glass(5),
        } {};

  void apply(AppSettings *settings);
  Glass getGlass(uint8_t index) const;

private:
  Glass glassess_[6];
};