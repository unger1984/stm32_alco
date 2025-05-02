#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateIdle::getType() const { return AppStateType::IDLE; }

void AppStateIdle::onEnter() {
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  // app.updatePump(false);
  // Обновим экран
  app.updateDisplay();
}

void AppStateIdle::onExit() {}

void AppStateIdle::onEvent(const ManagerEvent &event) {
  switch (event.source) {
  case ManagerEventSource::FROM_ENCODER:
    this->onEncoderEvent(event.data.encoder);
    break;
  default:
    break;
  }
  AppStateBase::onEvent(event);
}

void AppStateIdle::onEncoderEvent(const EncoderState &state) {
  if (state.type == EncoderEventType::RELEASE) {
    if (state.pressDurationMs > LONG_PRESS_DURATION) {
      // это было долгое нажатие, надо провалиться в меню
      app.switchState(&appStateMenu);
    } else {
      // это короткое нажатие, надо запустить задачу розлива
      app.switchState(&appStateWorker);
    }
  }
}

AppStateIdle appStateIdle;