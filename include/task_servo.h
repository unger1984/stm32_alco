#pragma once

#ifndef TASK_SERVO_H
#define TASK_SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <servo.h>

void taskServo_Init(void);
void taskServo_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_SERVO_H */