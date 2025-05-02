#include "AppContext.h"
#include "AppState.h"
#include "utils.h"

uint32_t startDrain = 0;
void runGlass(uint8_t index);

AppStateType AppStateWorker::getType() const { return AppStateType::WORKER; }

void AppStateWorker::onEnter() {
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
      app.getGlassess()->setStatePoured(index,
                                        app.getSettings()->data.doses[index]);
      app.updateDisplay();
      startDrain = 0;
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

void AppStateWorker::onTick() {
  for (uint8_t index = 0; index < 6; index++) {
    GlassState state = app.getGlassess()->getGlass(index);
    switch (state.type) {
    case GlassStateType::GLASS_DRAIN:
      app.getGlassess()->setStatePoured(
          index, map<uint32_t, uint8_t>(
                     millis() - startDrain, 0,
                     map<uint32_t>(app.getSettings()->data.doses[index], 0, 100,
                                   0, app.getSettings()->data.calibration),
                     0, app.getSettings()->data.doses[index]));
      app.updateDisplay();
      break;
    }
  }
}

void runGlass(uint8_t index) {
  index = clamp<uint8_t>(index, 0, 5);
  app.getGlassess()->setStateType(index, GlassStateType::GLASS_DRAIN);
  app.getGlassess()->setStatePoured(index, 0);
  // TODO сменить цвет светодиода
  WorkerEvent event = {
      .type = WorkerEventType::RUN,
      .angle = app.getSettings()->data.angles[index],
      .time = map<uint32_t>(app.getSettings()->data.doses[index], 0, 100, 0,
                            app.getSettings()->data.calibration),
  };
  startDrain = millis();
  // Обновим экран
  app.updateDisplay();
  app.updateWorker(&event);
}

AppStateWorker appStateWorker;