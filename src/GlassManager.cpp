#include "GlassManager.h"
#include "utils.h"

void GlassManager::apply(AppSettings *settings) {
  for (uint8_t index = 0; index < 6; index++) {
    glassess_[index].init(settings->angles[index],
                          map<uint32_t>(settings->doses[index], 0, 100, 0,
                                        100 * settings->calibration));
  }
}

Glass GlassManager::getGlass(uint8_t index) const { return glassess_[index]; };
