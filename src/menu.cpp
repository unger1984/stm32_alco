#include "Menu.h"
#include "utils.h"
// #include "settings.h"

const char *MenuItem::getName() const { return name; };
MenuItemType MenuItem::getType() const { return type; };
MenuItem *MenuItem::getParent() const { return parent; };
MenuItem **MenuItem::getChildrent() const { return children; };
uint8_t MenuItem::getSize() const { return size; };
bool MenuItem::isEqual(const char *value) {
  return isStringEqual(name, value);
};

MenuItem *mainChildren[] = {
    &menuDrain,
    &menuDosage,
    &menuServo,
    &menuCalibration,
};

MenuItem menuMain("Настройки", MenuItemType::Menu, nullptr, mainChildren, 4);

MenuItem menuDrain(MENU_DRAIN, MenuItemType::Action, &menuMain, nullptr, 0);

MenuItem menuDosageG1("Доза 1", MenuItemType::Edit, &menuDosage, nullptr, 0);
MenuItem menuDosageG2("Доза 2", MenuItemType::Edit, &menuDosage, nullptr, 0);
MenuItem menuDosageG3("Доза 3", MenuItemType::Edit, &menuDosage, nullptr, 0);
MenuItem menuDosageG4("Доза 4", MenuItemType::Edit, &menuDosage, nullptr, 0);
MenuItem menuDosageG5("Доза 5", MenuItemType::Edit, &menuDosage, nullptr, 0);
MenuItem menuDosageG6("Доза 6", MenuItemType::Edit, &menuDosage, nullptr, 0);

MenuItem *dosageChildren[] = {
    &menuDosageG1, &menuDosageG2, &menuDosageG3,
    &menuDosageG4, &menuDosageG5, &menuDosageG6,
};

MenuItem menuDosage(MENU_DOSAGE, MenuItemType::Menu, &menuMain, dosageChildren,
                    6);

MenuItem menuServoG1("Стопка 1", MenuItemType::Edit, &menuServo, nullptr, 0);
MenuItem menuServoG2("Стопка 2", MenuItemType::Edit, &menuServo, nullptr, 0);
MenuItem menuServoG3("Стопка 3", MenuItemType::Edit, &menuServo, nullptr, 0);
MenuItem menuServoG4("Стопка 4", MenuItemType::Edit, &menuServo, nullptr, 0);
MenuItem menuServoG5("Стопка 5", MenuItemType::Edit, &menuServo, nullptr, 0);
MenuItem menuServoG6("Стопка 6", MenuItemType::Edit, &menuServo, nullptr, 0);

MenuItem *servoChildren[] = {
    &menuServoG1, &menuServoG2, &menuServoG3,
    &menuServoG4, &menuServoG5, &menuServoG6,
};

MenuItem menuServo(MENU_SERVO, MenuItemType::Menu, &menuMain, servoChildren, 6);

MenuItem menuCalibration(MENU_CALIBRATION, MenuItemType::Action, &menuMain,
                         nullptr, 0);

void MenuState::setCurrent(MenuItem *menu) {
  current = menu;
  index = 0;
  selected = false;
};

MenuItem *MenuState::getCurrent() { return current; };
uint8_t MenuState::getIndex() { return index; };
void MenuState::setIndex(uint8_t value) { index = value; };
bool MenuState::isSelected() { return selected; }
void MenuState::setSelected(bool value) { selected = value; };

MenuState menuState(&menuMain, 0, 0);