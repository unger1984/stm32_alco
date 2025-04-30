#include "MenuItem.h"
#include "AppSettings.h"
#include "utils.h"

const char *MenuItem::getName() const { return name; };
MenuItemType MenuItem::getType() const { return type; };
MenuItem *MenuItem::getParent() const { return parent; };
MenuItem **MenuItem::getChildrent() const { return children; };
uint8_t MenuItem::getSize() const { return size; };
bool MenuItem::isEqual(const char *value) {
  return isStringEqual(name, value);
};
void *MenuItem::getParam() const { return settingsPtr; };

MenuItem *mainChildren[] = {
    &menuDrain, &menuDosage, &menuServo, &menuCalibration, &menuDebug,
};

MenuItem menuMain("Настройки", MenuItemType::MENU, nullptr, mainChildren, 5);

MenuItem menuDrain(MENU_DRAIN, MenuItemType::ACTION, &menuMain, nullptr, 0);

MenuItem menuDosageG1("Доза 1", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d1);
MenuItem menuDosageG2("Доза 2", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d1);
MenuItem menuDosageG3("Доза 3", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d2);
MenuItem menuDosageG4("Доза 4", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d3);
MenuItem menuDosageG5("Доза 5", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d4);
MenuItem menuDosageG6("Доза 6", MenuItemType::EDIT, &menuDosage, nullptr, 0,
                      &appSettings.doses.d5);

MenuItem *dosageChildren[] = {
    &menuDosageG1, &menuDosageG2, &menuDosageG3,
    &menuDosageG4, &menuDosageG5, &menuDosageG6,
};

MenuItem menuDosage(MENU_DOSAGE, MenuItemType::MENU, &menuMain, dosageChildren,
                    6);

MenuItem menuServoG1("Стопка 1", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a1);
MenuItem menuServoG2("Стопка 2", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a2);
MenuItem menuServoG3("Стопка 3", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a3);
MenuItem menuServoG4("Стопка 4", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a4);
MenuItem menuServoG5("Стопка 5", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a5);
MenuItem menuServoG6("Стопка 6", MenuItemType::EDIT, &menuServo, nullptr, 0,
                     &appSettings.angles.a6);

MenuItem *servoChildren[] = {
    &menuServoG1, &menuServoG2, &menuServoG3,
    &menuServoG4, &menuServoG5, &menuServoG6,
};

MenuItem menuServo(MENU_SERVO, MenuItemType::MENU, &menuMain, servoChildren, 6);

MenuItem menuCalibration(MENU_CALIBRATION, MenuItemType::ACTION, &menuMain,
                         nullptr, 0);

MenuItem menuDebug(MENU_DEBUG, MenuItemType::EDIT, &menuMain, nullptr, 0,
                   &appSettings.isDebug);
