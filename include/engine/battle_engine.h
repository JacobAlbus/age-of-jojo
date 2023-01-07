#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_

#include "engine/game_state.h"
#include "engine/base_controller.h"
#include "cinder/app/App.h"
//#include "engine/player_controller.h"
//#include "engine/ai_controller.h"

namespace age_of_jojo {

class BattleEngine {
 public:

  BattleEngine();

  void HandlePlayer1Input(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner);

  void HandlePlayer2Input(const glm::vec2& top_right_corner);

  inline const GameState& GetGameState() const { return game_state_; };

  void RenderAllUnits(const glm::vec2& top_right_corner) const;

  void RenderBases(const glm::vec2& top_right_corner) const;

  void UpdateUnitPositions(const glm::vec2& top_right_corner);

 private:
  BaseController* player1_controller_;
  BaseController* player2_controller_;
  GameState game_state_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
