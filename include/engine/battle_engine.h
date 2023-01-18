#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_

#include "engine/game_state.h"
#include "engine/base_controller.h"
#include "cinder/app/App.h"
#include <unordered_map>
//#include "engine/player_controller.h"
//#include "engine/ai_controller.h"

namespace age_of_jojo {

class BattleEngine {
 public:

  BattleEngine();

  BattleEngine(const BattleEngine& src);

  ~BattleEngine();

  BattleEngine& operator=(const BattleEngine& rhs);


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

  /**
   * Handles mouse click
   * @param event data on mouse placement
   */
  void HandleMouseClick(const ci::app::MouseEvent& event);

  void RenderPlayer1HUD() const;

  void HandleKeyDown(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner);

  void RenderPlayer1Queue(const glm::vec2& top_right_corner) const;

  GameMode GetGameMode() const;

  void RestartGame();

 private:
  /**
   * Update positions of players
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitPositions(const glm::vec2& top_right_corner);

  bool CannotBuildUnit(ControllerAction player_action, bool is_team_jojo) const;

  /**
   * Update healths of units from attacks
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitsHealth(const glm::vec2& top_right_corner);

  /**
   * Checks for and removes dead players, maintains order of units
   */
  void RemoveDeadUnits();

  float GetMaxBaseHealth(Era era) const;

  /**
   * Perform actions for player 2
   * @param top_right_corner coordinates of top right corner of map
   */
  void HandlePlayer2Input(const glm::vec2& top_right_corner);

  /**
   * Perform actions for player 1
   * @param top_right_corner coordinates of top right corner of map
   */
  void HandlePlayer1Input(const glm::vec2& top_right_corner);

  /**
   * Updates base of health after getting attacked
   */
  void UpdateBaseHealth(const glm::vec2& top_right_corner);

  BaseController* player1_controller_;
  BaseController* player2_controller_;
  GameState game_state_;

  std::unordered_map<std::string, ci::gl::TextureRef> hud_images_;
  const std::unordered_map<Era, std::unordered_map<UnitType, int>> kUnitCosts_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
