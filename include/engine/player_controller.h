#pragma once
#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_

#include "engine/base_controller.h"

namespace age_of_jojo {

class PlayerController : public BaseController {
 public:

  ControllerAction CalculateCurrentAction(const GameState& current_state, bool is_team_jojo) override;

  inline double GetTimerSeconds() const override { return 1.0; };

 private:
  static bool IsMouseInsideImage(const ci::Rectf& image_hitbox, const glm::vec2& mouse_pos);
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_
