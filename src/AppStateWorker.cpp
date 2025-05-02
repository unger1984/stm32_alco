#include "AppContext.h"
#include "AppState.h"
#include "utils.h"

void runGlass(uint8_t index);

AppStateType AppStateWorker::getType() const { return AppStateType::WORKER; }

void AppStateWorker::onEnter() {
  // Обновим экран
  app.updateDisplay();
  // TODO заглушка
  runGlass(5);
}

void AppStateWorker::onExit() {}

void AppStateWorker::onEvent(const ManagerEvent &event) {
  switch (event.source) {
  case ManagerEventSource::FROM_ENCODER:
    this->onEncoderEvent(event.data.encoder);
    break;
  case ManagerEventSource::FROM_WORKER:
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
  for (uint8_t index = 0; index < 6; index++) {
    GlassState state = app.getGlassess()->getGlass(index);
    switch (state.type) {
    case GlassStateType::GLASS_DRAIN:
      // В эту стопку закончили наливать
      app.getGlassess()->setStateType(index, GlassStateType::GLASS_FULL);
      // TODO сменить цвет светодиода
      break;
    case GlassStateType::GLASS_EMPTY:
      // наткнулись на пустую стопку
      runGlass(index);
      return; // дальше ничего не делаем, уже задачу отправили
    default:
      break;
    }
  }
  app.updateServo(0);
  app.switchState(&appStateIdle);
}

void runGlass(uint8_t index) {
  index = clamp<uint8_t>(index, 0, 5);
  app.getGlassess()->setStateType(index, GlassStateType::GLASS_DRAIN);
  // TODO сменить цвет светодиода
  WorkerEvent event = {
      .type = WorkerEventType::RUN,
      .angle = app.getSettings()->data.angles[index],
      .time = map<uint32_t>(app.getSettings()->data.doses[index], 0, 100, 0,
                            app.getSettings()->data.calibration),
  };
  app.updateWorker(&event);
}

AppStateWorker appStateWorker;