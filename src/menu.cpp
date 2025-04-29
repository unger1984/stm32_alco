#include "menu.h"

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
    .name = "Прокачка",
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
};

MenuItem_t menuDosageG2 = {
    .name = "Доза 2",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosageG3 = {
    .name = "Доза 3",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosageG4 = {
    .name = "Доза 4",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosageG5 = {
    .name = "Доза 5",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosageG6 = {
    .name = "Доза 6",
    .type = MenuItemType_t::Settings,
    .parent = &menuDosage,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosage = {
    .name = "Дозы по рюмкам",
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
    .name = "Рюмка 1",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServoG2 = {
    .name = "Рюмка 2",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServoG3 = {
    .name = "Рюмка 3",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServoG4 = {
    .name = "Рюмка 4",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServoG5 = {
    .name = "Рюмка 5",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServoG6 = {
    .name = "Рюмка 6",
    .type = MenuItemType_t::Settings,
    .parent = &menuServo,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServo = {
    .name = "Углы серво",
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
    .name = "Калибровка",
    .type = MenuItemType_t::Action,
    .parent = &menuMain,
    .children = NULL,
    .size = 0,
};