#include "engine/player_controller.h"
#include "engine/consts.h"

namespace age_of_jojo {

ControllerAction PlayerController::CalculateCurrentAction(const GameState& current_state, bool is_team_jojo) {
  glm::vec2 mouse_pos = current_state.GetMouse().getPos();

  if (mouse_pos == game_values::kForbiddenMousePos_) {
    return ControllerAction::kIdle;
  } else if (IsMouseInsideImage(game_values::kInfantryButtonPos_, mouse_pos)) {
    return ControllerAction::kSendInfantry;
  } else if (IsMouseInsideImage(game_values::kSlingerButtonPos_, mouse_pos)) {
    return ControllerAction::kSendSlinger;
  } else if (IsMouseInsideImage(game_values::kHeavyButtonPos_, mouse_pos)) {
    return kSendHeavy;
  } else if (IsMouseInsideImage(game_values::kUpgradeButtonPos_, mouse_pos)) {
    return kUpgrade;
  }

  return ControllerAction::kIdle;
}

bool PlayerController::IsMouseInsideImage(const ci::Rectf& image_hitbox, const glm::vec2& mouse_pos) {
  return (mouse_pos.x > image_hitbox.getX1() && mouse_pos.x < image_hitbox.getX2() &&
          mouse_pos.y > image_hitbox.getY1() && mouse_pos.y < image_hitbox.getY2());
}

}

