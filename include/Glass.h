#pragma once

#include <stdint.h>

enum class GlassState {
  NONE = 0, // нет стопки
  EMPTY,    // пустая стопка
  DRAIN,    // стопка наливается
  FULL,     // стопка налита
};

class Glass {
public:
  Glass(uint8_t index) : index_(index), angle_(0), time_(0) {};

  uint8_t getIndex() const;
  GlassState getType() const;
  void changeState(GlassState state);
  void init(uint8_t angle, uint32_t time);
  uint8_t getAngle() const;
  uint32_t getTome() const;

private:
  GlassState state_ = GlassState::NONE;
  uint8_t index_;
  uint8_t angle_;
  uint32_t time_;
};