#include "menu.h"
#include "settings.h"

MenuItem_t menuMain = {
    .name = "Настройки",
    .type = MenuItemType_t::Menu,
    .parent = NULL,
    .children =
        (MenuItem_t *[]){
            &menuDrain,
            &menuDosage,
            &menuServo,
            &menuCalibration,
        },
    .size = 4,
};

MenuItem_t menuDrain = {
    .name = MENU_DRAIN,
    .type = MenuItemType_t::Action,
    .parent = &menuMain,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosageG1 = {
    .name = "Доза 1",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d1,
};

MenuItem_t menuDosageG2 = {
    .name = "Доза 2",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d2,
};

MenuItem_t menuDosageG3 = {
    .name = "Доза 3",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d3,
};

MenuItem_t menuDosageG4 = {
    .name = "Доза 4",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d4,
};

MenuItem_t menuDosageG5 = {
    .name = "Доза 5",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d5,
};

MenuItem_t menuDosageG6 = {
    .name = "Доза 6",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.doses.d6,
};

MenuItem_t menuDosage = {
    .name = MENU_DOSAGE,
    .type = MenuItemType_t::Menu,
    .parent = &menuMain,
    .children =
        (MenuItem_t *[]){
            &menuDosageG1,
            &menuDosageG2,
            &menuDosageG3,
            &menuDosageG4,
            &menuDosageG5,
            &menuDosageG6,
        },
    .size = 6,
};

MenuItem_t menuServoG1 = {
    .name = "Стопка 1",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a1,
};

MenuItem_t menuServoG2 = {
    .name = "Стопка 2",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a2,
};

MenuItem_t menuServoG3 = {
    .name = "Стопка 3",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a3,
};

MenuItem_t menuServoG4 = {
    .name = "Стопка 4",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a4,
};

MenuItem_t menuServoG5 = {
    .name = "Стопка 5",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a5,
};

MenuItem_t menuServoG6 = {
    .name = "Стопка 6",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
    .settingsPtr = &currentSettings.angles.a6,
};

MenuItem_t menuServo = {
    .name = MENU_SERVO,
    .type = MenuItemType_t::Menu,
    .parent = &menuMain,
    .children =
        (MenuItem_t *[]){
            &menuServoG1,
            &menuServoG2,
            &menuServoG3,
            &menuServoG4,
            &menuServoG5,
            &menuServoG6,
        },
    .size = 6,
};

MenuItem_t menuCalibration = {
    .name = MENU_CALIBRATION,
    .type = MenuItemType_t::Action,
    .parent = &menuMain,
    .children = NULL,
    .size = 0,
};