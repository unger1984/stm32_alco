#pragma once

#ifndef TASK_OLED_H
#define TASK_OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <oled.h>

void taskOled_Init(void);
void taskOled_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_OLED_H */