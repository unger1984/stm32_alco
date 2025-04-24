#pragma once

#ifndef TASK_ENCODER_H
#define TASK_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <encoder.h>

void taskEncoder_Init(void);
void taskEncoder_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_ENCODER_H */