#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_

#include "engine/base_controller.h"

namespace age_of_jojo {

class PlayerController : public BaseController {
 public:
  ControllerAction CalculateCurrentAction(const GameState& current_state) override;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_PLAYER_CONTROLLER_H_
