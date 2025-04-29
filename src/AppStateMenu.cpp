#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateMenu::getType() const { return AppStateType::MENU; }
void AppStateMenu::onEnter() {
  // Запаркуем серво
  app.updateServo(0);
  // Остановим насос
  app.updatePump(0);
  // Обновим экран
  app.getMenu()->setCurrent(&menuMain);
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
  case EncoderEventType::Release:
    if (state.pressDurationMs >= LONG_PRESS_DURATION) {
      this->onLongPress(state);
    } else {
      this->onClick(state);
    }
    break;
  case EncoderEventType::Rotate:
    this->onRotate(state);
    break;
  case EncoderEventType::Press:
    // if (isStringEqueal(currentState.menu.current->name, "Прокачка")) {
    //   currentState.hold = 0;
    //   osThreadFlagsSet(taskDisplayHandle, 0x01);
    // }
    break;
  case EncoderEventType::Hold:
    this->onHold(state);
    break;
  default:
    break;
  }
}

void AppStateMenu::onLongPress(const EncoderState &state) {
  if (app.getMenu()->getCurrent()->getParent() == nullptr) {
    // Если это главное меню, то выходим
    app.switchState(&appStateIdle);
  } else {
    switch (app.getMenu()->getCurrent()->getType()) {
    case MenuItemType::Menu:
    case MenuItemType::Edit:
      if (!app.getMenu()->isSelected()) {
        // Это подменю
        // или не настройка которую редактируют в данный момент
        // надо на уровень вверх
        app.getMenu()->setCurrent(app.getMenu()->getCurrent()->getParent());
        app.updateDisplay();
      }
      break;
    case MenuItemType::Action:
      if (app.getMenu()->getCurrent()->isEqual(MENU_DRAIN)) {
        // отпустили после прокачки
        // выключить помпу!
        app.updatePump(false);
        app.setHold(0);
        app.updateDisplay();
      } else if (app.getMenu()->getCurrent()->isEqual(MENU_CALIBRATION)) {
        // отпустили после калибровки
        // выключить помпу!
        app.updatePump(false);
        app.setHold(state.pressDurationMs);
        app.updateDisplay();
      }
      break;
    default:
      break;
    }
  }
};

void AppStateMenu::onClick(const EncoderState &state) {
  switch (app.getMenu()->getCurrent()->getType()) {
  case MenuItemType::Menu:
    // Мы находимся в списке подразделов
    if (app.getMenu()->getCurrent()->getSize() > 0) {
      // смотрим на какой пункт наведен курсор
      MenuItem *indexMenu = app.getMenu()
                                ->getCurrent()
                                ->getChildrent()[app.getMenu()->getIndex()];
      switch (indexMenu->getType()) {
      case MenuItemType::Menu:
      case MenuItemType::Action:
        // это подменю или действие, надло провалитья
        app.getMenu()->setCurrent(indexMenu);
        if (indexMenu->isEqual(MENU_DRAIN)) {
          // Это мы вошли в прокачку, значит надо повернуть серво
          app.setHold(0);
          app.updateServo(90);
        } else if (indexMenu->isEqual(MENU_CALIBRATION)) {
          // Это мы вошли в калибровку, значит надо повернуть серво
          // app.setHold(currentState.currentSettings->calibration);
          app.updateServo(90);
        }
        app.updateDisplay();
        break;
      case MenuItemType::Edit:
        // Это настройка
        if (app.getMenu()->isSelected()) {
          // снимем выделение
          app.getMenu()->setSelected(false);
          if (app.getMenu()->getCurrent()->isEqual(MENU_SERVO)) {
            app.updateServo(0);
          }
        } else {
          // выделим
          app.getMenu()->setSelected(true);
          if (app.getMenu()->getCurrent()->isEqual(MENU_SERVO)) {
            // uint8_t angle = *(uint8_t *)indexMenu->settingsPtr;
            // osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
          }
        }
        osThreadFlagsSet(taskDisplayHandle, 0x01);
        break;
      }
    }
    break;
  case MenuItemType::Action:
    // Это пункт Action значит по одинарному клику возвращаемся назад
    if (app.getMenu()->getCurrent()->isEqual(MENU_DRAIN)) {
      // Это мы выходим из прокачки, значит надо повернуть серво
      app.setHold(0);
      app.updateServo(0);
    } else if (app.getMenu()->getCurrent()->isEqual(MENU_CALIBRATION)) {
      // Это мы выходим из калибровки, значит надо повернуть серво
      // currentState.currentSettings->calibration = currentState.hold;
      app.setHold(0);
      app.updateServo(0);
    }
    app.getMenu()->setCurrent(app.getMenu()->getCurrent()->getParent());
    app.updateDisplay();
    break;
  default:
    break;
  }
};

void AppStateMenu::onHold(const EncoderState &state) {
  if (app.getMenu()->getCurrent()->isEqual(MENU_DRAIN) ||
      app.getMenu()->getCurrent()->isEqual(MENU_CALIBRATION)) {
    // Если помпа не включена - включить
    if (!app.isPump()) {
      app.updatePump(1);
    }
    app.setHold(state.pressDurationMs);
    app.updateDisplay();
  }
};

void AppStateMenu::onRotate(const EncoderState &state) {
  switch (app.getMenu()->getCurrent()->getType()) {
  case MenuItemType::Menu:
    // Это подменю
    if (app.getMenu()->isSelected()) {
      // MenuItem *itemMenu = app.getMenu()
      //                          ->getCurrent()
      //                          ->getChildrent()[app.getMenu()->getIndex()];
      // uint8_t val = *(uint8_t *)itemMenu->settingsPtr;
      // int newval = val + event.steps * (event.press ? 10 : 1);
      // if (isStringEqueal(currentState.menu.current->name, MENU_SERVO)) {
      //   if (newval > 180)
      //     newval = 180;
      //   if (newval < 0)
      //     newval = 0;
      //   uint8_t angle = newval;
      //   osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
      // } else if (isStringEqueal(currentState.menu.current->name,
      // MENU_DOSAGE)) {
      //   if (newval > 100)
      //     newval = 100;
      //   if (newval < 0)
      //     newval = 0;
      // }
      // *(uint8_t
      // *)currentState.menu.current->children[currentState.menu.index]
      //      ->settingsPtr = newval;
      // osThreadFlagsSet(taskDisplayHandle, 0x01);
    } else {
      // выделения нет - перемещаемся между пунктами
      uint8_t size = app.getMenu()->getCurrent()->getSize();
      int8_t index = app.getMenu()->getIndex() + state.steps;
      if (index >= size - 1) {
        index = size - 1;
      } else if (index < 0) {
        index = 0;
      }
      app.getMenu()->setIndex(index);
      app.updateDisplay();
    }
    break;
  default:
    break;
  }
};

AppStateMenu appStateMenu;