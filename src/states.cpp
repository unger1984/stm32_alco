#include "states.h"
#include "menu.h"
#include "utils.h"

#define LONG_PRESS_DURATION 500

void handleStateMenuEncoder(EncoderState_t event);

/// @brief Главный обработчик состояния Idle
/// @param event
void handleStateIdle(ManagerEvent_t event) {
  switch (event.source) {
  case ManagerEventSource_t::ENCODER:
    // от энекодеро просто обрабатываем короткое и длинное нажатие
    if (event.data.encoder.type == EncoderEvent_t::Release) {
      if (event.data.encoder.pressDurationMs > LONG_PRESS_DURATION) {
        // это было долгое нажатие, надо провалиться в меню
        currentState.type = AppStateType_t::MENU;
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

/// @brief Обработка длтинного нажатия в меню
/// @param event
void handleStateMenuEncoder_LongPress(EncoderState_t event) {
  if (currentState.menu.current->parent == NULL) {
    // Если это главное меню, то выходим
    currentState.type = AppStateType_t::IDLE;
    osThreadFlagsSet(taskDisplayHandle, 0x01);
  } else {
    switch (currentState.menu.current->type) {
    case MenuItemType_t::Menu:
    case MenuItemType_t::Settings:
      if (!currentState.menu.isSelected) {
        // Это подменю
        // или не настройка которую редактируют в данный момент
        // надо на уровень вверх
        currentState.menu = {
            .current = currentState.menu.current->parent,
            .index = 0,
            .isSelected = 0,
        };
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      }
      break;
    case MenuItemType_t::Action:
      if (isStringEqueal(currentState.menu.current->name, MENU_DRAIN)) {
        // отпустили после прокачки
        // выключить помпу!
        uint8_t pump = 0;
        osMessageQueuePut(queuePumpHandle, &pump, 0, osWaitForever);
        currentState.hold = 0;
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      } else if (isStringEqueal(currentState.menu.current->name,
                                MENU_CALIBRATION)) {
        // отпустили после калибровки
        // выключить помпу!
        uint8_t pump = 0;
        osMessageQueuePut(queuePumpHandle, &pump, 0, osWaitForever);
        currentState.hold = event.pressDurationMs;
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      }
      break;
    default:
      break;
    }
  }
}

/// @brief Обработка короткого нажатия в меню
/// @param event
void handleStateMenuEncoder_ShortPress(EncoderState_t event) {
  switch (currentState.menu.current->type) {
  case MenuItemType_t::Menu:
    // Мы находимся в списке подразделов
    if (currentState.menu.current->size > 0) {
      // смотрим на какой пункт наведен курсор
      MenuItem_t *indexMenu =
          currentState.menu.current->children[currentState.menu.index];
      switch (indexMenu->type) {
      case MenuItemType_t::Menu:
      case MenuItemType_t::Action: {
        // это подменю или действие, надло провалитья
        currentState.menu = {
            .current = indexMenu,
            .index = 0,
            .isSelected = 0,
        };
        if (isStringEqueal(indexMenu->name, MENU_DRAIN)) {
          // Это мы вошли в прокачку, значит надо повернуть серво
          currentState.hold = 0;
          uint8_t angle = 90;
          osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
        } else if (isStringEqueal(indexMenu->name, MENU_CALIBRATION)) {
          // Это мы вошли в калибровку, значит надо повернуть серво
          currentState.hold = currentState.currentSettings->calibration;
          uint8_t angle = 90;
          osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
        }
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      } break;
      case MenuItemType_t::Settings: {
        // Это настройка
        if (currentState.menu.isSelected) {
          // снимем выделение
          currentState.menu.isSelected = 0;
          if (isStringEqueal(currentState.menu.current->name, MENU_SERVO)) {
            uint8_t angle = 0;
            osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
          }
        } else {
          // выделим
          currentState.menu.isSelected = 1;
          if (isStringEqueal(currentState.menu.current->name, MENU_SERVO)) {
            uint8_t angle = *(uint8_t *)indexMenu->settingsPtr;
            osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
          }
        }
        osThreadFlagsSet(taskDisplayHandle, 0x01);
      } break;
      }
    }
    break;
  case MenuItemType_t::Action:
    // Это пункт Action значит по одинарному клику возвращаемся назад
    if (isStringEqueal(currentState.menu.current->name, MENU_DRAIN)) {
      // Это мы выходим из прокачки, значит надо повернуть серво
      currentState.hold = 0;
      uint8_t angle = 0;
      osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
    } else if (isStringEqueal(currentState.menu.current->name,
                              MENU_CALIBRATION)) {
      // Это мы выходим из калибровки, значит надо повернуть серво
      currentState.currentSettings->calibration = currentState.hold;
      currentState.hold = 0;
      uint8_t angle = 0;
      osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
    }
    currentState.menu = {
        .current = currentState.menu.current->parent,
        .index = 0,
        .isSelected = 0,
    };
    osThreadFlagsSet(taskDisplayHandle, 0x01);
    break;
  default:
    break;
  }
}

/// @brief Обработка вращения энкодера в меню
/// @param event
void handleStateMenuEncoder_Rotate(EncoderState_t event) {
  switch (currentState.menu.current->type) {
  case MenuItemType_t::Menu:
    // Это подменю
    if (currentState.menu.isSelected) {
      MenuItem_t *itemMenu =
          currentState.menu.current->children[currentState.menu.index];
      uint8_t val = *(uint8_t *)itemMenu->settingsPtr;
      int newval = val + event.steps * (event.press ? 10 : 1);
      if (isStringEqueal(currentState.menu.current->name, MENU_SERVO)) {
        if (newval > 180)
          newval = 180;
        if (newval < 0)
          newval = 0;
        uint8_t angle = newval;
        osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
      } else if (isStringEqueal(currentState.menu.current->name, MENU_DOSAGE)) {
        if (newval > 100)
          newval = 100;
        if (newval < 0)
          newval = 0;
      }
      *(uint8_t *)currentState.menu.current->children[currentState.menu.index]
           ->settingsPtr = newval;
      osThreadFlagsSet(taskDisplayHandle, 0x01);
    } else {
      // выделения нет - перемещаемся между пунктами
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

/// @brief Обработка событий энкодера в меню
/// @param event
void handleStateMenuEncoder(EncoderState_t event) {
  switch (event.type) {
  case EncoderEvent_t::Release:
    if (event.pressDurationMs >= LONG_PRESS_DURATION) {
      handleStateMenuEncoder_LongPress(event);
    } else {
      handleStateMenuEncoder_ShortPress(event);
    }
    break;
  case EncoderEvent_t::Rotate:
    handleStateMenuEncoder_Rotate(event);
    break;
  case EncoderEvent_t::Press:
    // if (isStringEqueal(currentState.menu.current->name, "Прокачка")) {
    //   currentState.hold = 0;
    //   osThreadFlagsSet(taskDisplayHandle, 0x01);
    // }
    break;
  case EncoderEvent_t::Hold:
    if (isStringEqueal(currentState.menu.current->name, MENU_DRAIN) ||
        isStringEqueal(currentState.menu.current->name, MENU_CALIBRATION)) {
      // Если помпа не включена - включить
      if (!currentState.pump) {
        uint8_t pump = 1;
        osMessageQueuePut(queuePumpHandle, &pump, 0, osWaitForever);
      }
      currentState.hold = event.pressDurationMs;
      osThreadFlagsSet(taskDisplayHandle, 0x01);
    }
    break;
  default:
    break;
  }
}