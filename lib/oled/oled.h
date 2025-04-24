#pragma once

#include "i2c.h"
#include <u8g2.h>

#define I2C_ADDRESS 0x3C // I2C address of the OLED display
#define WIDTH 128        // Максимальное количество точек
#define HEIGHT 64

#ifdef __cplusplus

class OLED {
public:
  OLED();
  void init();
  void setFont(const uint8_t *font);
  void print(u8g2_uint_t x, u8g2_uint_t y, const char *str);
  void update();
  void clear();
  void clearAll();
  void setColor(uint8_t color);
  void box(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);

  void drowGraph(float temp);

private:
  static uint8_t byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr);
  static uint8_t gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                   void *arg_ptr);

  void addToBuffer(int value);

  u8g2_t u8g2;
  float buffer[WIDTH] = {0};
  int buffer_size = 0;
};

#endif