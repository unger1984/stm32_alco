#ifndef _STATES_H
#define _STATES_H

#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

void handleStateIdle(ManagerEvent_t event);
void handleStateWork(ManagerEvent_t event);
void handleStateMenu(ManagerEvent_t event);

#ifdef __cplusplus
}
#endif

#endif /* _STATES_H */