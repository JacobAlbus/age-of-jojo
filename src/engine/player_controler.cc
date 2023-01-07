#include "engine/player_controller.h"

namespace age_of_jojo {

ControllerAction PlayerController::CalculateCurrentAction(const GameState &current_state) {

  return ControllerAction::kIdle;
}

}

