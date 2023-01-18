#include "engine/ai_controller.h"
#include <cstdlib>
#include <random>

namespace age_of_jojo {

AIController::AIController() : timer_(ci::Timer()) {
  timer_.start();
}

ControllerAction AIController::CalculateCurrentAction(const GameState& current_state) {
  if (timer_.getSeconds() > 5.0) {
    std::default_random_engine e((unsigned int)time(nullptr));
    int i = e() % 3;

    timer_.stop();
    timer_.start();
    return static_cast<ControllerAction>(i);
  } else {
    return kIdle;
  }
}

}
