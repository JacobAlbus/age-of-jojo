#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_

#include "engine/base_controller.h"

namespace age_of_jojo {

class AIController : public BaseController {
 public:

  AIController();

  ControllerAction CalculateCurrentAction(const GameState& current_state) override;

 private:
  int enemy_score_;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_
