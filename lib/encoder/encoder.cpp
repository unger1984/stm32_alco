#include "encoder.h"

Encoder::Encoder(TIM_HandleTypeDef *timer, GPIO_TypeDef *buttonPort,
                 uint16_t buttonPin)
    : timer_(timer), buttonPort_(buttonPort), buttonPin_(buttonPin),
      lastCounter_(0), lastButtonState_(GPIO_PIN_SET), buttonPressTime_(0),
      buttonWasPressedDuringRotate_(false), currentEvent_{} {}

void Encoder::Init() {
  HAL_TIM_Encoder_Start(timer_, TIM_CHANNEL_ALL);
  lastCounter_ = __HAL_TIM_GET_COUNTER(timer_);
  lastButtonState_ = HAL_GPIO_ReadPin(buttonPort_, buttonPin_);
}

void Encoder::Update() {
  HandleRotation();
  HandleButton();
}

bool Encoder::HasEvent() const {
  return currentEvent_.type != EncoderEventType::None;
}

EncoderEventType Encoder::GetEventType() const { return currentEvent_.type; }

EncoderEvent Encoder::GetEvent() {
  EncoderEvent e = currentEvent_;
  currentEvent_ = EncoderEvent{}; // сброс
  return e;
}

void Encoder::HandleRotation() {
  uint16_t current = __HAL_TIM_GET_COUNTER(timer_) >> 1;

  if (lastCounter_ != current) {
    int8_t res = (current > lastCounter_) ? 1 : -1;
    lastCounter_ = current;
    currentEvent_.type = EncoderEventType::Rotate;
    currentEvent_.ticks = res;
    currentEvent_.buttonPressedDuring =
        HAL_GPIO_ReadPin(buttonPort_, buttonPin_) == GPIO_PIN_RESET;
  }
}

void Encoder::HandleButton() {
  GPIO_PinState state = HAL_GPIO_ReadPin(buttonPort_, buttonPin_);

  if (lastButtonState_ == GPIO_PIN_SET && state == GPIO_PIN_RESET) {
    // начало клика
    buttonPressTime_ = HAL_GetTick();
    lastDuration_ = 0;
    currentEvent_.type = EncoderEventType::Press;
    currentEvent_.pressDurationMs = 0;
  } else if (lastButtonState_ == GPIO_PIN_RESET && state == GPIO_PIN_SET) {
    // отпустили
    currentEvent_.pressDurationMs = HAL_GetTick() - buttonPressTime_;
    currentEvent_.type = EncoderEventType::Release;
    currentEvent_.type =
        (currentEvent_.pressDurationMs < SHORT_PRESS_THRESHOLD_MS)
            ? EncoderEventType::Click
            : EncoderEventType::LongClick;
  } else if (lastButtonState_ == GPIO_PIN_RESET && state == GPIO_PIN_RESET) {
    // удерживают
    currentEvent_.pressDurationMs = HAL_GetTick() - buttonPressTime_;
    if (currentEvent_.pressDurationMs > SHORT_PRESS_THRESHOLD_MS) {
      currentEvent_.type = EncoderEventType::Hold;
      lastDuration_ = currentEvent_.pressDurationMs;
    }
  }

  lastButtonState_ = state;
}
