#pragma once
#ifndef _APP_STATE_H
#define _APP_STATE_H

#include "app_shared.h"

enum class AppStateType {
  LOADING = 0,
  IDLE,
  WORK,
  MENU,
};

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

class AppStateIdle : public AppStateBase {
public:
  AppStateType getType() const;
  void onEnter();
  void onExit();
  void onEvent(const ManagerEvent &event);

private:
  void onEncoderEvent(const EncoderState &state);
};

class AppStateMenu : public AppStateBase {
public:
  AppStateType getType() const;
  void onEnter();
  void onExit();
  void onEvent(const ManagerEvent &event);

private:
  void onEncoderEvent(const EncoderState &state);
  void onLongPress(const EncoderState &state);
  void onClick(const EncoderState &state);
  void onHold(const EncoderState &state);
  void onRotate(const EncoderState &state);
};

extern AppStateIdle appStateIdle;
extern AppStateMenu appStateMenu;

#endif /* _APP_STATE_H */