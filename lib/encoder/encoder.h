#pragma once
#include "tim.h"

#ifdef __cplusplus

enum class EncoderEventType { None, Rotate, Click, Release };

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
  bool buttonWasPressedDuringRotate_;

  EncoderEvent currentEvent_;
};

#endif