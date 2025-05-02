#include "AppContext.h"
#include "AppState.h"

AppStateType AppStateWorker::getType() const { return AppStateType::WORKER; }

void AppStateWorker::onEnter() {
  // Обновим экран
  app.updateDisplay();
  // TODO заглушка
  WorkerEvent event = {
      .type = WorkerEventType::RUN,
      .angle = 90,
      .time = 5000,
  };
  app.updateWorker(&event);
}

void AppStateWorker::onExit() {}

void AppStateWorker::onEvent(const ManagerEvent &event) {
  switch (event.source) {
  case ManagerEventSource::ENCODER:
    this->onEncoderEvent(event.data.encoder);
    break;
  case ManagerEventSource::WORKER:
    this->onWorkerDone();
    break;
  default:
    break;
  }
  AppStateBase::onEvent(event);
}

void AppStateWorker::onEncoderEvent(const EncoderState &state) {
  if (state.type == EncoderEventType::RELEASE) {
    // Было нажатие, значит надо остановить воркер
    WorkerEvent event = {.type = WorkerEventType::STOP};
    app.updateWorker(&event);
  }
}

void AppStateWorker::onWorkerDone() {
  // воркер закончил
  // TODO проверить наличие новых заданий
  app.updateServo(0);
  app.switchState(&appStateIdle);
}

AppStateWorker appStateWorker;