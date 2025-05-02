#include "oled.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T> T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

OLED::OLED() {}

void OLED::init() {
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, byte_cb,
                                         gpio_and_delay_cb);
  u8g2_SetI2CAddress(&u8g2, I2C_ADDRESS << 1);

  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
}

void OLED::setFont(const uint8_t *font) { u8g2_SetFont(&u8g2, font); }

void OLED::print(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
  u8g2_DrawUTF8(&u8g2, x, y, str);
}

void OLED::update() { u8g2_SendBuffer(&u8g2); }

void OLED::clear() { u8g2_ClearBuffer(&u8g2); }
void OLED::clearAll() { u8g2_ClearDisplay(&u8g2); }

void OLED::setColor(uint8_t color) { u8g2_SetDrawColor(&u8g2, color); }

void OLED::box(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) {
  u8g2_DrawBox(&u8g2, x, y, w, h);
}

void OLED::frame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) {
  u8g2_DrawFrame(&u8g2, x, y, w, h);
}

void OLED::drowGraph(float temp) {
  addToBuffer(temp);

  this->clear();
  for (int i = 0; i < (buffer_size < WIDTH ? buffer_size : WIDTH); i++) {
    int x = map(i, 0, (buffer_size < WIDTH ? buffer_size : WIDTH), 0,
                buffer_size < WIDTH ? buffer_size : WIDTH);
    int y = map(buffer[i], -40.0f, 100.0f, (float)HEIGHT, 0.0f);
    u8g2_DrawPixel(&u8g2, x, y);
  }
}

// - - Private Functions ---

void OLED::addToBuffer(int value) {
  if (buffer_size > 0) {
    if (buffer[buffer_size - 1] == value)
      return; // Если значение уже есть в буфере, не добавляем его
    if (abs(value - buffer[buffer_size - 1]) > 5) {
      return; // Если разница между значениями больше 5, не добавляем
    }
  }

  if (buffer_size < WIDTH) {
    buffer[buffer_size++] = value;
  } else {
    // Сдвигаем всё влево
    for (int i = 1; i < WIDTH; i++) {
      buffer[i - 1] = buffer[i];
    }
    buffer[WIDTH - 1] = value;
  }
}

uint8_t OLED::getUTF8Width(const char *val) {
  return u8g2_GetUTF8Width(&u8g2, val);
}

void OLED::setFontPosBaseline() { u8g2_SetFontPosBaseline(&u8g2); };
void OLED::setFontPosBottom() { u8g2_SetFontPosBottom(&u8g2); };
void OLED::setFontPosTop() { u8g2_SetFontPosTop(&u8g2); };
void OLED::setFontPosCenter() { u8g2_SetFontPosCenter(&u8g2); };

// --- Static Callbacks ---

uint8_t OLED::byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                      void *arg_ptr) {
  static uint8_t buffer[32];
  static uint8_t buf_idx;
  uint8_t *data;

  switch (msg) {
  case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;
    while (arg_int--)
      buffer[buf_idx++] = *data++;
    break;

  case U8X8_MSG_BYTE_START_TRANSFER:
    buf_idx = 0;
    break;

  case U8X8_MSG_BYTE_END_TRANSFER:
    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS << 1, buffer, buf_idx, 20);
    break;
  }
  return 1;
}

uint8_t OLED::gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                void *arg_ptr) {
  switch (msg) {
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_DELAY_I2C:
    for (volatile int i = 0; i < arg_int * 12; i++)
      __NOP();
    break;
  }
  return 1;
}