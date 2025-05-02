#pragma once

#ifndef _APP_CONTEXT_H
#define _APP_CONTEXT_H

#include "AppState.h"
#include "MenuItem.h"
#include "MenuManager.h"
#include "SettingsManager.h"
#include "app_shared.h"
#include <cmsis_os.h>

class AppContext {
public:
  void switchState(AppStateBase *newState);
  void handleEvent(const ManagerEvent &event);

  /// @brief обновить экран
  void updateDisplay();
  /// @brief Повернуть серво
  /// @param angle угол поворота
  void updateServo(uint8_t angle);
  /// @brief Включить/выключить помпу
  /// @param pump 0/1
  void updatePump(bool pump);
  /// @brief Отправить событие воркеру
  /// @param event
  void updateWorker(const WorkerEvent *event);

  /// @brief Отправить статус помпы
  void sendPumpStatus(uint8_t pump);
  /// @brief Отправить статус серво
  void sendServoStatus(uint8_t angle);
  /// @brief Отправить об окончании задания
  void sendWorkerDone();

  /// @brief Текущий угол поворота сервы
  uint8_t getServo() const;
  void setServo(uint8_t value);
  /// @brief Текущее состояние помпы 1/0 включено/выключено
  uint8_t isPump() const;
  void setPump(uint8_t value);
  /// @brief сколько времени держат нажатым энкодер
  uint32_t getHold() const;
  void setHold(uint32_t value);
  SettingsManager *getSettings();

  AppStateBase *getState() const;
  MenuManager *getMenu() const;

private:
  AppStateBase *currentState = nullptr;
  uint8_t servo = 0;
  bool pump = 0;
  uint32_t hold = 0;
  MenuManager *menu = &appManuManager;
  SettingsManager *settings = &appSettings;
};

extern AppContext app;

#endif /* _APP_CONTEXT_H */