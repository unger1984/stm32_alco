#include "app_shared.h"
#include "tim.h"
#include "utils.h"
#include <cmsis_os2.h>
#include <stdio.h>

#define MIN_HOLD_DURATION 500 // минимальное время после считается удержанно

void TaskEncoder(void *argument) {
  uint16_t lastCounter_;
  GPIO_PinState lastButtonState_;

  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

  lastCounter_ = __HAL_TIM_GET_COUNTER(&htim4);
  lastButtonState_ = HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin);
  uint32_t buttonPressTime_ = 0;

  for (;;) {
    EncoderState state = EncoderState{};
    state.type = EncoderEventType::NONE;

    GPIO_PinState btn = HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin);
    if (lastButtonState_ == GPIO_PIN_SET && btn == GPIO_PIN_RESET) {
      // Начало нажатия
      buttonPressTime_ = millis();
      state.type = EncoderEventType::PRESS;
      state.pressDurationMs = 0;
      state.press = 1;
    } else if (lastButtonState_ == GPIO_PIN_RESET && btn == GPIO_PIN_SET) {
      // Отпустили кнопку
      state.pressDurationMs = millis() - buttonPressTime_;
      state.type = EncoderEventType::RELEASE;
      state.press = 0;
    } else if (lastButtonState_ == GPIO_PIN_RESET && btn == GPIO_PIN_RESET) {
      // Держать кнопку
      if (millis() - buttonPressTime_ >= MIN_HOLD_DURATION) {
        state.type = EncoderEventType::HOLD;
        state.pressDurationMs = millis() - buttonPressTime_;
        state.press = 1;
      }
    }
    lastButtonState_ = btn;

    // Проверим было ли вращение
    uint16_t current = __HAL_TIM_GET_COUNTER(&htim4) >> 1;
    if (lastCounter_ != current) {
      state.type = EncoderEventType::ROTATE;
      state.steps = (current < lastCounter_) ? 1 : -1;
      lastCounter_ = current;
      state.press = lastButtonState_ == GPIO_PIN_RESET ? 1 : 0;
    }

    if (state.type != EncoderEventType::NONE) {
      ManagerEvent event;
      event.source = ManagerEventSource::FROM_ENCODER;
      event.data.encoder = state;
      osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
    }

    osDelay(10);
  }
}