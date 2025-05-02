#include "GlassManager.h"
#include "utils.h"

void GlassManager::setStatePoured(uint8_t index, uint8_t value) {
  glassess_[index].poured = value;
};

void GlassManager::setStateType(uint8_t index, GlassStateType type) {
  glassess_[index].type = type;
};

GlassState GlassManager::getGlass(uint8_t index) const {
  return glassess_[index];
};

GlassManager glassManager;