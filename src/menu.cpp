#include "menu.h"

MenuItem_t menuMain = {
    .name = "Настройки",
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
    .parent = NULL,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuDosage = {
    .name = "Дозы по рюмкам",
    .parent = NULL,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuServo = {
    .name = "Углы серво",
    .parent = NULL,
    .children = NULL,
    .size = 0,
};

MenuItem_t menuCalibration = {
    .name = "Калибровка",
    .parent = NULL,
    .children = NULL,
    .size = 0,
};