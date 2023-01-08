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
  /**
   * Perform actions for player 1
   * @param event key being pressed down
   * @param top_right_corner coordinates of top right corner of map
   */
  void HandlePlayer1Input(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner);

  /**
   * Updates state of game every frame
   */
  void UpdateGameState(const glm::vec2& top_right_corner);

  /**
   * Render character units
   * @param top_right_corner
   */
  void RenderAllUnits(const glm::vec2& top_right_corner) const;

  /**
   * Renders hit box and hit bar of bases
   * @param top_right_corner coordinates of top right corner of map
   */
  void RenderBases(const glm::vec2& top_right_corner) const;

  inline const GameState& GetGameState() const { return game_state_; };

 private:
  /**
   * Update positions of players
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitPositions(const glm::vec2& top_right_corner);

  /**
   * Update healths of units from attacks
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitsHealth(const glm::vec2& top_right_corner);

  /**
   * Checks for and removes dead players, maintains order of units
   */
  void RemoveDeadUnits();

  /**
   * Perform actions for player 2
   * @param top_right_corner coordinates of top right corner of map
   */
  void HandlePlayer2Input(const glm::vec2& top_right_corner);

  /**
   * Updates base of health after getting attacked
   */
  void UpdateBaseHealth(const glm::vec2& top_right_corner);

  BaseController* player1_controller_;
  BaseController* player2_controller_;
  GameState game_state_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
