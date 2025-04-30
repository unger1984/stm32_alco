#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateLoading::getType() const { return AppStateType::LOADING; }

void AppStateLoading::onEnter() {
  // Обновим экран
  app.updateDisplay();
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  app.updatePump(0);
  // TODO загрузка настроек и данных
  osDelay(1000 * 5);
  // Все загрузили, ждем командыы
  app.switchState(&appStateIdle);
}

void AppStateLoading::onExit() {}

void AppStateLoading::onEvent(const ManagerEvent &event) {
  // Никак не реагируем на события
}

AppStateLoading appStateLoading;