#include "AppContext.h"
#include "AppState.h"

void AppStateBase::onEvent(const ManagerEvent &event) {
  // Тут поработаем с событиями которые обрабатываются одинаково везде
  switch (event.source) {
  case ManagerEventSource::SERVO:
    app.setServo(event.data.servo);
    break;
  case ManagerEventSource::PUMP:
    app.setPump(event.data.pump);
    break;
  // TODO обработать событие добавления/удаления стопки
  default:
    break;
  }
};