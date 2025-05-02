#include "AppContext.h"

void AppContext::switchState(AppStateBase *newState) {
  if (currentState)
    currentState->onExit();
  currentState = newState;
  if (currentState)
    currentState->onEnter();
}

void AppContext::handleEvent(const ManagerEvent &event) {
  if (currentState)
    currentState->onEvent(event);
}

/// @brief обновить экран
void AppContext::updateDisplay() { osThreadFlagsSet(taskDisplayHandle, 0x01); }

/// @brief Повернуть серво
/// @param angle угол поворота
void AppContext::updateServo(uint8_t angle) {
  osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
}

/// @brief Включить/выключить помпу
/// @param pump 0/1
void AppContext::updatePump(bool pump) {
  osMessageQueuePut(queuePumpHandle, &pump, 0, osWaitForever);
}

/// @brief Отправить событие воркеру
/// @param event
void AppContext::updateWorker(WorkerEvent *event) {
  osMessageQueuePut(queueWorkerHandle, event, 0, osWaitForever);
}

/// @brief Отправить статус помпы
void AppContext::sendPumpStatus(uint8_t pump) {
  ManagerEvent event = {
      .source = PUMP,
      .data =
          {
              .pump = pump,
          },
  };
  osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
}

/// @brief Отправить статус серво
void AppContext::sendServoStatus(uint8_t angle) {
  ManagerEvent event = {
      .source = SERVO,
      .data =
          {
              .servo = angle,
          },
  };
  osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
}

/// @brief Отправить об окончании задания
void AppContext::sendWorkerDone() {
  ManagerEvent event = {
      .source = WORKER,
  };
  osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
}

/// @brief Текущий угол поворота сервы
uint8_t AppContext::getServo() const { return servo; }
void AppContext::setServo(uint8_t value) { servo = value; }

/// @brief Текущее состояние помпы 1/0 включено/выключено
uint8_t AppContext::isPump() const { return pump; }
void AppContext::setPump(uint8_t value) { pump = value; }

/// @brief сколько времени держат нажатым энкодер
uint32_t AppContext::getHold() const { return hold; }
void AppContext::setHold(uint32_t value) { hold = value; }

AppStateBase *AppContext::getState() const { return currentState; }

MenuManager *AppContext::getMenu() const { return menu; };

AppSettings *AppContext::getSettings() { return settings; };

AppContext app;