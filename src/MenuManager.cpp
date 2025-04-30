#include "MenuManager.h"
#include "AppContext.h"
#include "debug.h"
#include "utils.h"

MenuItem *MenuManager::getCurrent() { return current; };
uint8_t MenuManager::getIndex() { return index; };
bool MenuManager::isSelected() { return selected; };
void MenuManager::changeCurrent(MenuItem *item) {
  current = item;
  index = 0;
  selected = false;
};

void MenuManager::onClick() {
  switch (current->getType()) {
  case MenuItemType::MENU:
    // Мы находимся в списке подразделов
    if (current->getSize() > 0) {
      // смотрим на какой пункт наведен курсор
      MenuItem *indexMenu = current->getChildrent()[index];
      switch (indexMenu->getType()) {
      case MenuItemType::MENU:
      case MenuItemType::ACTION:
        // это подменю или действие, надло провалитья
        changeCurrent(indexMenu);
        if (indexMenu->isEqual(MENU_DRAIN)) {
          // Это мы вошли в прокачку, значит надо повернуть серво
          app.setHold(0);
          app.updateServo(90);
        } else if (indexMenu->isEqual(MENU_CALIBRATION)) {
          // Это мы вошли в калибровку, значит надо повернуть серво
          app.setHold(app.getSettings()->calibration);
          app.updateServo(90);
        }
        app.updateDisplay();
        break;
      case MenuItemType::EDIT:
        // Это настройка
        if (selected) {
          // снимем выделение
          selected = false;
          if (app.getMenu()->getCurrent()->isEqual(MENU_SERVO)) {
            app.updateServo(0);
          } else if (indexMenu->isEqual(MENU_DEBUG)) {
            // включим или выключи отладку
            handleDebug(app.getSettings()->isDebug);
          }
        } else {
          // выделим
          selected = true;
          if (current->isEqual(MENU_SERVO)) {
            app.updateServo(*(uint8_t *)indexMenu->getParam());
          }
        }
        app.updateDisplay();
        break;
      }
    }
    break;
  case MenuItemType::ACTION:
    // Это пункт Action значит по одинарному клику возвращаемся назад
    if (current->isEqual(MENU_DRAIN)) {
      // Это мы выходим из прокачки, значит надо повернуть серво
      app.setHold(0);
      app.updateServo(0);
    } else if (current->isEqual(MENU_CALIBRATION)) {
      // Это мы выходим из калибровки, значит надо повернуть серво
      app.getSettings()->calibration = app.getHold();
      app.setHold(0);
      app.updateServo(0);
    }
    changeCurrent(current->getParent());
    app.updateDisplay();
    break;
  default:
    break;
  }
}

void MenuManager::onLongPress(uint32_t duration) {
  if (current->getParent() == nullptr) {
    // Если это главное меню, то выходим
    app.switchState(&appStateIdle);
  } else {
    switch (current->getType()) {
    case MenuItemType::MENU:
    case MenuItemType::EDIT:
      if (!selected) {
        // Это подменю
        // или не настройка которую редактируют в данный момент
        // надо на уровень вверх
        changeCurrent(current->getParent());
        app.updateDisplay();
      }
      break;
    case MenuItemType::ACTION:
      if (current->isEqual(MENU_DRAIN)) {
        // отпустили после прокачки
        // выключить помпу!
        app.updatePump(false);
        app.setHold(0);
        app.updateDisplay();
      } else if (current->isEqual(MENU_CALIBRATION)) {
        // отпустили после калибровки
        // выключить помпу!
        app.updatePump(false);
        app.setHold(duration);
        app.updateDisplay();
      }
      break;
    default:
      break;
    }
  }
}

void MenuManager::onHold(uint32_t duration) {
  if (current->isEqual(MENU_DRAIN) || current->isEqual(MENU_CALIBRATION)) {
    // Если помпа не включена - включить
    if (!app.isPump()) {
      app.updatePump(1);
    }
    app.setHold(duration);
    app.updateDisplay();
  }
}

void MenuManager::onRotate(int steps, bool pressed) {
  switch (current->getType()) {
  case MenuItemType::MENU:
    // Это подменю
    if (selected) {
      MenuItem *itemMenu = current->getChildrent()[index];
      uint8_t val = *(uint8_t *)itemMenu->getParam();
      int newval = val + steps * (pressed ? 10 : 1);
      if (current->isEqual(MENU_SERVO)) {
        newval = clamp(newval, 0, 180);
        app.updateServo(newval);
      } else if (current->isEqual(MENU_DOSAGE)) {
        newval = clamp(newval, 0, 100);
      } else if (itemMenu->isEqual(MENU_DEBUG)) {
        newval = clamp<int8_t>(newval, 0, 1);
      }
      *(uint8_t *)current->getChildrent()[index]->getParam() = newval;
      app.updateDisplay();
    } else {
      // выделения нет - перемещаемся между пунктами
      uint8_t size = current->getSize();
      index = clamp<uint8_t>(max(0, index + steps), 0, size - 1);
      app.updateDisplay();
    }
    break;
  default:
    break;
  }
}

MenuManager appManuManager;