#include "app.h"
#include "tim.h"
#include <cmsis_os2.h>
#include <stdio.h>

#define MIN_HOLD_DURATION 50 // минимальное время после считается удержанно

void TaskEncoder(void *argument) {
  uint16_t lastCounter_;
  GPIO_PinState lastButtonState_;

  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

  lastCounter_ = __HAL_TIM_GET_COUNTER(&htim4);
  lastButtonState_ = HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin);
  uint32_t buttonPressTime_ = 0;

  for (;;) {
    EncoderState_t state = EncoderState_t{};
    state.type = EncoderEvent_t::None;

    GPIO_PinState btn = HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin);
    if (lastButtonState_ == GPIO_PIN_SET && btn == GPIO_PIN_RESET) {
      // Начало нажатия
      buttonPressTime_ = millis();
      state.type = EncoderEvent_t::Press;
      state.pressDurationMs = 0;
      state.press = 1;
    } else if (lastButtonState_ == GPIO_PIN_RESET && btn == GPIO_PIN_SET) {
      // Отпустили кнопку
      state.pressDurationMs = millis() - buttonPressTime_;
      state.type = EncoderEvent_t::Release;
      state.press = 0;
    } else if (lastButtonState_ == GPIO_PIN_RESET && btn == GPIO_PIN_RESET) {
      // Держать кнопку
      if (millis() - buttonPressTime_ >= MIN_HOLD_DURATION) {
        state.type = EncoderEvent_t::Hold;
        state.pressDurationMs = millis() - buttonPressTime_;
        state.press = 1;
      }
    }
    lastButtonState_ = btn;

    // Проверим было ли вращение
    uint16_t current = __HAL_TIM_GET_COUNTER(&htim4) >> 1;
    if (lastCounter_ != current) {
      state.type = EncoderEvent_t::Rotate;
      state.steps = (current < lastCounter_) ? 1 : -1;
      lastCounter_ = current;
      state.press = lastButtonState_ == GPIO_PIN_RESET ? 1 : 0;
    }

    if (state.type != EncoderEvent_t::None) {
      //   printf("Event: %d Steps: %d Pressed %d Durations: %ul\r\n",
      //   state.type,
      //          state.steps, state.press, state.pressDurationMs);

      ManagerEvent_t event;
      event.source = ManagerEventSource_t::ENCODER;
      event.data.encoder = state;
      osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
    }

    osDelay(10);
  }
}