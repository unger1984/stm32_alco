#include "Glass.h"

uint8_t Glass::getIndex() const { return index_; };
GlassState Glass::getType() const { return state_; };
void Glass::changeState(GlassState state) { state_ = state; };
void Glass::init(uint8_t angle, uint32_t time) {
  angle_ = angle;
  time_ = time;
};
uint8_t Glass::getAngle() const { return angle_; };
uint32_t Glass::getTome() const { return time_; };