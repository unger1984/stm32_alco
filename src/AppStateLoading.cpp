#include "AppContext.h"
#include "AppState.h"
#include "debug.h"

AppStateType AppStateLoading::getType() const { return AppStateType::LOADING; }

void AppStateLoading::onEnter() {
  // Обновим экран
  app.updateDisplay();
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  app.updatePump(0);
  // Загрузка настроек и данных
  app.getSettings()->load();
  handleDebug(app.getSettings()->data.isDebug);

  // Все загрузили, ждем командыы
  app.switchState(&appStateIdle);
}

void AppStateLoading::onExit() {}

void AppStateLoading::onEvent(const ManagerEvent &event) {
  AppStateBase::onEvent(event);
}

AppStateLoading appStateLoading;