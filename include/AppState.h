#pragma once
#ifndef _APP_STATE_H
#define _APP_STATE_H

#include "app_shared.h"

/// @brief Тип текущего состяния
enum class AppStateType {
  LOADING = 0,
  IDLE,
  WORKER,
  MENU,
};

/// @brief Базовый класс состояния
class AppStateBase {
public:
  /// @brief Тип текущего состяния для идентификации
  virtual AppStateType getType() const = 0;

  /// @brief Выполняется при входе в состояние
  virtual void onEnter() = 0;

  /// @brief Выполняется при выходе из состояния
  virtual void onExit() = 0;

  /// @brief Выполняется при событии
  virtual void onEvent(const ManagerEvent &event);

  virtual ~AppStateBase() = default;
};

/// @brief Состояние загрузки
class AppStateLoading : public AppStateBase {
public:
  AppStateType getType() const override;
  void onEnter() override;
  void onExit() override;
  void onEvent(const ManagerEvent &event) override;
};

/// @brief Состояние ожидания команд
class AppStateIdle : public AppStateBase {
public:
  AppStateType getType() const override;
  void onEnter() override;
  void onExit() override;
  void onEvent(const ManagerEvent &event) override;

private:
  void onEncoderEvent(const EncoderState &state);
};

/// @brief Состояние "В работе"
class AppStateWorker : public AppStateBase {
public:
  AppStateType getType() const override;
  void onEnter() override;
  void onExit() override;
  void onEvent(const ManagerEvent &event);

private:
  void onEncoderEvent(const EncoderState &state);
  void onWorkerDone();
};

/// @brief Состояние Меню
class AppStateMenu : public AppStateBase {
public:
  AppStateType getType() const override;
  void onEnter() override;
  void onExit() override;
  void onEvent(const ManagerEvent &event);

private:
  void onEncoderEvent(const EncoderState &state);
};

extern AppStateLoading appStateLoading;
extern AppStateIdle appStateIdle;
extern AppStateMenu appStateMenu;
extern AppStateWorker appStateWorker;

#endif /* _APP_STATE_H */