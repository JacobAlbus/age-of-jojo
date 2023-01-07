#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_

#include "engine/game_state.h"

namespace age_of_jojo {

enum ControllerAction {
  kIdle,
  kSendInfantry,
  kSendSlinger,
  kSendHeavy,
  kBuildTurret,
  kBuyTurretSmall,
  kBuyTurretMedium,
  kBuyTurretBig
};

class BaseController {
 public:
  virtual ControllerAction CalculateCurrentAction(const GameState& current_state) = 0;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_
