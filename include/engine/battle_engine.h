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

  /**
   * Render player 1 UI features
   */
  void RenderPlayer1HUD() const;

  /**
   * Performs action based on key clicked
   * @param event key being pressed
   * @param top_right_corner top right cornr of map
   */
  void HandleKeyDown(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner);

  /**
   * Renders player 1 Queue
   * @param top_right_corner top right corner of map
   */
  void RenderPlayer1Queue(const glm::vec2& top_right_corner) const;

  /**
   * @return Current game mode, either a player has won or we're still playing
   */
  GameMode GetGameMode() const;

  /**
   * Restarts game state, giving both players a clean slate
   */
  void RestartGame();

  /**
   * Renders how much a unit costs
   * @param mouse_pos position of mouse
   */
  void RenderUnitCost(const glm::vec2& mouse_pos) const;

 private:
  /**
   * Update positions of players
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitPositions(const glm::vec2& top_right_corner);

  /**
   * @param image_hitbox image box
   * @param mouse_pos position of mouse in 2d coords
   * @return Whether or not the mouse is inside the specified box
   */
  static bool IsMouseInsideImage(const ci::Rectf& image_hitbox, const glm::vec2& mouse_pos);

  /**
   * @param player_action unit wanting to be built
   * @param is_team_jojo whether or not we're looking at Jojo or Dio
   * @return whether or not a player has enough money to build unit
   */
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

  /**
   * @param era Era in which player is in
   * @return maximum base health for a given era
   */
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

  /**
   * Updates the game mode depending on if a player won
   */
  void UpdateGameMode();

  /**
   * Counts number of already stored games in directory
   */
  size_t GetPlayedGamesCount();

  BaseController* player1_controller_;
  BaseController* player2_controller_;
  GameState game_state_;
  ci::gl::BatchRef unit_health_bar_;
  GameMode game_mode_;

  std::string json_file_path_;
  ci::Timer write_state_timer_;

  size_t jojo_wins_;
  size_t dio_wins_;

  std::unordered_map<std::string, ci::gl::TextureRef> hud_images_;
  const std::unordered_map<Era, std::unordered_map<UnitType, int>> kUnitCosts_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BATTLE_ENGINE_H_
