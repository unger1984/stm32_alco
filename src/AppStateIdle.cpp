#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateIdle::getType() const { return AppStateType::IDLE; }

void AppStateIdle::onEnter() {
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  app.updatePump(0);
  // Обновим экран
  app.updateDisplay();
}

void AppStateIdle::onExit() {}

void AppStateIdle::onEvent(const ManagerEvent &event) {
  switch (event.source) {
  case ManagerEventSource::ENCODER:
    this->onEncoderEvent(event.data.encoder);
    break;
  default:
    break;
  }
  AppStateBase::onEvent(event);
}

void AppStateIdle::onEncoderEvent(const EncoderState &state) {
  if (state.type == EncoderEventType::Release) {
    if (state.pressDurationMs > LONG_PRESS_DURATION) {
      // это было долгое нажатие, надо провалиться в меню
      app.switchState(&appStateMenu);
      // Тут надо запарковать серво в 0 и нарисовать меню
      uint8_t angle = 0;
      osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
      osThreadFlagsSet(taskDisplayHandle, 0x01);
    } else {
      // это короткое нажатие, надо запустить задачу розлива
      // TODO реализовать
    }
  }
}

AppStateIdle appStateIdle;