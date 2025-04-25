/**
 * @file encoder.h
 * @brief Encoder class for handling rotary encoders with button
 * @details This class provides methods to initialize, update, and handle
 * @anchor Andrey Unger unger1984@gmail.com
 *
 *  Это упрощенный класс для работы с энкодерами с кнопкой.
 *  Он использует таймеры STM32 для отслеживания вращения и состояния кнопки.
 *
 * Не проверял портируемость и совместимость!
 */

#pragma once
#include "tim.h"

#define SHORT_PRESS_THRESHOLD_MS 500
#define ROTATION_STOP_DELAY_MS 50

#ifdef __cplusplus

enum class EncoderEventType {
  None,
  Rotate,
  Click,
  LongClick,
  Press,
  Release,
  Hold
};

struct EncoderEvent {
  EncoderEventType type = EncoderEventType::None;
  int16_t ticks = 0;                // для Rotate
  bool buttonPressedDuring = false; // для Rotate
  uint32_t pressDurationMs = 0;     // для Release
};

class Encoder {
public:
  Encoder(TIM_HandleTypeDef *timer, GPIO_TypeDef *buttonPort,
          uint16_t buttonPin);

  void Init();
  void Update();

  bool HasEvent() const;
  EncoderEventType GetEventType() const;
  EncoderEvent GetEvent();

private:
  void HandleRotation();
  void HandleButton();

  TIM_HandleTypeDef *timer_;
  GPIO_TypeDef *buttonPort_;
  uint16_t buttonPin_;

  uint16_t lastCounter_;
  GPIO_PinState lastButtonState_;
  uint32_t buttonPressTime_;
  uint32_t lastDuration_;
  bool buttonWasPressedDuringRotate_;

  EncoderEvent currentEvent_;
};

#endif