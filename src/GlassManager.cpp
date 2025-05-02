#include "GlassManager.h"
#include "utils.h"

void GlassManager::setStateType(uint8_t index, GlassStateType type) {
  glassess_[index].type = type;
};

GlassState GlassManager::getGlass(uint8_t index) const {
  return glassess_[index];
};

GlassManager glassManager;