#include "states.h"
#include "menu.h"

void handleStateMenuEncoder(EncoderState_t event);

/// @brief Главный обработчик состояния Idle
/// @param event
void handleStateIdle(ManagerEvent_t event) {
  switch (event.source) {
  case ManagerEventSource_t::ENCODER:
    // от энекодеро просто обрабатываем короткое и длинное нажатие
    if (event.data.encoder.type == EncoderEvent_t::Release) {
      if (event.data.encoder.pressDurationMs > 1000) {
        // это было долгое нажатие, надо провалиться в меню
        currentState.type = AppStateType_t::Menu;
        currentState.menu = {
            .current = &menuMain,
            .index = 0,
            .isSelected = 0,
        };
        // Тут надо запарковать серво в 0 и нарисовать меню
        uint8_t angle = 0;
        osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      } else {
        // это короткое нажатие, надо запустить задачу розлива
        // TODO реализовать
      }
    }
    break;

  default:
    break;
  }
}

void handleStateWork(ManagerEvent_t event) {}

/// @brief Главный обработчик Состояния меню
/// @param event
void handleStateMenu(ManagerEvent_t event) {
  switch (event.source) {
  case ManagerEventSource_t::ENCODER:
    handleStateMenuEncoder(event.data.encoder);
    break;

  default:
    break;
  }
}

void handleStateMenuEncoder(EncoderState_t event) {
  switch (event.type) {
  case EncoderEvent_t::Release:
    if (event.pressDurationMs >= 1000) {
      // длинное нажатие
      if (currentState.menu.current->parent == NULL) {
        // Если это главное меню, то выходим
        currentState.type = AppStateType_t::Idle;
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      }
    }
    break;
  case EncoderEvent_t::Rotate:
    if (currentState.menu.current->size > 0) {
      // Это меню с дочерними пунктами, значит просто перемещаемся между ними
      int8_t index = currentState.menu.index + event.steps;
      if (index >= currentState.menu.current->size - 1) {
        index = currentState.menu.current->size - 1;
      } else if (index < 0) {
        index = 0;
      }
      currentState.menu.index = index;
      osThreadFlagsSet(taskDisplayHandle, 0x01);
    }
    break;
  default:
    break;
  }
}