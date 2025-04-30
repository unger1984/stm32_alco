#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateMenu::getType() const { return AppStateType::MENU; }
void AppStateMenu::onEnter() {
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  app.updatePump(0);
  // Обновим экран
  app.updateDisplay();
}
void AppStateMenu::onExit() {}

void AppStateMenu::onEvent(const ManagerEvent &event) {
  if (event.source == ManagerEventSource::ENCODER) {
    this->onEncoderEvent(event.data.encoder);
  }
  AppStateBase::onEvent(event);
}

void AppStateMenu::onEncoderEvent(const EncoderState &state) {
  switch (state.type) {
  case EncoderEventType::RELEASE:
    if (state.pressDurationMs >= LONG_PRESS_DURATION) {
      app.getMenu()->onLongPress(state.pressDurationMs);
    } else {
      app.getMenu()->onClick();
    }
    break;
  case EncoderEventType::ROTATE:
    app.getMenu()->onRotate(state.steps, state.press);
    break;
  case EncoderEventType::PRESS:
    // if (isStringEqueal(currentState.menu.current->name, "Прокачка")) {
    //   currentState.hold = 0;
    //   osThreadFlagsSet(taskDisplayHandle, 0x01);
    // }
    break;
  case EncoderEventType::HOLD:
    app.getMenu()->onHold(state.pressDurationMs);
    break;
  default:
    break;
  }
}

AppStateMenu appStateMenu;