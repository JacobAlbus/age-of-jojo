#include "engine/ai_controller.h"

namespace age_of_jojo {

AIController::AIController() : enemy_score_(0) {

}

ControllerAction AIController::CalculateCurrentAction(const GameState& current_state) {
  enemy_score_ = 1;
//  for (const Unit& enemy_unit : current_state.GetJojoUnits()) {
//    enemy_score_ += 0;
//  }

  if (enemy_score_ % 100 == 0) {
    enemy_score_ += 20;
    return ControllerAction::kSendInfantry;
  } else {
    return ControllerAction::kIdle;
  }
}

}
