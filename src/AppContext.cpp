#include "AppContext.h"

void AppContext::switchState(AppStateBase *newState) {
  if (currentState)
    currentState->onExit();
  currentState = newState;
  if (currentState)
    currentState->onEnter();
}

void AppContext::handleEvent(const ManagerEvent &event) {
  if (currentState)
    currentState->onEvent(event);
}

void AppContext::updateDisplay() { osThreadFlagsSet(taskDisplayHandle, 0x01); }
void AppContext::updateServo(uint8_t angle) {
  osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
}
void AppContext::updatePump(bool pump) {
  osMessageQueuePut(queuePumpHandle, &pump, 0, osWaitForever);
}

uint8_t AppContext::getServo() const { return servo; }
void AppContext::setServo(uint8_t value) { servo = value; }

uint8_t AppContext::isPump() const { return pump; }
void AppContext::setPump(uint8_t value) { pump = value; }
uint32_t AppContext::getHold() const { return hold; }
void AppContext::setHold(uint32_t value) { hold = value; }

AppStateBase *AppContext::getState() const { return currentState; }

MenuManager *AppContext::getMenu() const { return menu; };

AppSettings *AppContext::getSettings() { return settings; };

// Settings *AppContext::getSettings() { return &settings; };

AppContext app;