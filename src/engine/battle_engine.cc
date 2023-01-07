#include "engine/battle_engine.h"
#include "engine/player_controller.h"
#include "engine/ai_controller.h"
#include "engine/styles.h"

namespace age_of_jojo {

BattleEngine::BattleEngine() : game_state_(GameState()),
                               player1_controller_(new PlayerController()),
                               player2_controller_(new AIController()) { }

void BattleEngine::HandlePlayer1Input(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner) {
//  ControllerAction player2_action = player2_controller_->CalculateCurrentAction(game_state_);

//  switch (player2_action) {
//    case ControllerAction::kSendInfantry:
//      game_state_.AddJojoUnit(UnitType::Infantry);
//      break;
//    case ControllerAction::kSendSlinger:
//      game_state_.AddJojoUnit(UnitType::Slinger);
//      break;
//    case ControllerAction::kSendHeavy:
//      game_state_.AddJojoUnit(UnitType::Heavy);
//      break;
//  }
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_i:
      game_state_.AddJojoUnit(UnitType::Infantry, top_right_corner);
      break;
    case ci::app::KeyEvent::KEY_s:
      game_state_.AddDioUnit(UnitType::Slinger, top_right_corner);
      break;
    case ci::app::KeyEvent::KEY_h:
      game_state_.AddJojoUnit(UnitType::Heavy, top_right_corner);
      break;
  }
}

void BattleEngine::HandlePlayer2Input(const glm::vec2& top_right_corner) {
  ControllerAction player2_action = player2_controller_->CalculateCurrentAction(game_state_);

  if (player2_action == ControllerAction::kSendInfantry) {
    game_state_.AddDioUnit(UnitType::Infantry, top_right_corner);
  }
}

void BattleEngine::UpdateUnitPositions(const glm::vec2& top_right_corner) {
  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
                                  styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                  styles::kBaseLength_ + top_right_corner.x,
                                  styles::kBackgroundHeight_);

  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);

  game_state_.UpdatePositions(top_right_corner, kJojoBaseCoords, kDioBaseCoords);
}

void BattleEngine::RenderBases(const glm::vec2& top_right_corner) const {
//  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
//                                  background_vertical - styles::kBaseHeight_,
//                                  styles::kBaseLength_ + top_right_corner.x,
//                                  background_vertical);
//
//  const ci::Rectf kDioBaseCoords(background_horizontal - styles::kBaseLength_ + top_right_corner.x,
//                                 background_vertical - styles::kBaseHeight_,
//                                 background_horizontal + top_right_corner.x,
//                                 background_vertical);
}

void BattleEngine::RenderAllUnits(const glm::vec2& top_right_corner) const {
  for (const auto& unit_pair : game_state_.GetJojoUnits()) {
    ci::gl::color(ci::Color("white"));
    unit_pair.first.RenderUnit(top_right_corner);
  }
  for (const auto& unit_pair : game_state_.GetDioUnits()) {
    ci::gl::color(ci::Color("black"));
    unit_pair.first.RenderUnit(top_right_corner);
  }
}

}

