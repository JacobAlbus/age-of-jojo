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

void BattleEngine::UpdateGameState(const glm::vec2& top_right_corner) {
  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
                                  styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                  styles::kBaseLength_ + top_right_corner.x,
                                  styles::kBackgroundHeight_);

  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);

  UpdateBaseHealth(top_right_corner); // Health related checks have to go first
  UpdateUnitsHealth(top_right_corner);

  game_state_.UpdateCanUnitAttack(top_right_corner, kJojoBaseCoords, kDioBaseCoords);
  RemoveDeadUnits();
  HandlePlayer2Input(top_right_corner);
  UpdateUnitPositions(top_right_corner);
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

void BattleEngine::UpdateBaseHealth(const glm::vec2& top_right_corner) {
  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
                                  styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                  styles::kBaseLength_ + top_right_corner.x,
                                  styles::kBackgroundHeight_);
  bool is_attack_jojo_base = true;

  for (const auto& unit_pair : game_state_.GetDioUnits()) {
    if (unit_pair.first.CheckCollision(kJojoBaseCoords, top_right_corner) &&
        unit_pair.first.CanAttack()) {
      game_state_.UpdateBaseHealth(unit_pair.first.GetAttackPower(), is_attack_jojo_base);
    }
  }

  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);
  is_attack_jojo_base = false;

  for (const auto& unit_pair : game_state_.GetJojoUnits()) {
    if (unit_pair.first.CheckCollision(kDioBaseCoords, top_right_corner) &&
        unit_pair.first.CanAttack()) {
      game_state_.UpdateBaseHealth(unit_pair.first.GetAttackPower(), is_attack_jojo_base);
    }
  }
}

void BattleEngine::UpdateUnitsHealth(const glm::vec2& top_right_corner) {

  ci::gl::drawStringCentered("nibbie",
                             glm::vec2(500, 200),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 60));

  game_state_.UpdateUnitsHealth(top_right_corner);
}

void BattleEngine::RemoveDeadUnits() {
  game_state_.RemoveDeadUnits();
}

//void BattleEngine::UpdateCanUnitAttack(const glm::vec2 &top_right_corner) {
//  for (auto& unit_pair : game_state_.GetDioUnits()) {
//    if (unit_pair.first.CanAttack(top_right_corner, jojo_base_coords, dio_base_coords)) {
//      game_state_.UpdateBaseHealth(unit_pair.first.GetAttackPower(), is_attack_jojo_base);
//    }
//  }
//}

void BattleEngine::RenderBases(const glm::vec2& top_right_corner) const {
  // Draw Jojo health bar
  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
                                  styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                  styles::kBaseLength_ + top_right_corner.x,
                                  styles::kBackgroundHeight_);
  ci::gl::color(ci::Color("red"));
  ci::gl::drawSolidRect(ci::Rectf(kJojoBaseCoords.getX1(), kJojoBaseCoords.getY1() - 20,
                                  kJojoBaseCoords.getX2(), kJojoBaseCoords.getY1() - 10));

  ci::gl::color(ci::Color("green"));
  float health_percentage = game_state_.GetJojoHealth() / 10000;
  ci::gl::drawSolidRect(ci::Rectf(kJojoBaseCoords.getX1(), kJojoBaseCoords.getY1() - 20,
                                  kJojoBaseCoords.getX1() + (styles::kBaseLength_ * health_percentage), kJojoBaseCoords.getY1() - 10));

  // Draw Dio health bar
  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);

  ci::gl::color(ci::Color("red"));
  ci::gl::drawSolidRect(ci::Rectf(kDioBaseCoords.getX1(), kDioBaseCoords.getY1() - 20,
                                  kDioBaseCoords.getX2(), kDioBaseCoords.getY1() - 10));

  ci::gl::color(ci::Color("green"));
  health_percentage = game_state_.GetDioHealth() / 10000;
  ci::gl::drawSolidRect(ci::Rectf(kDioBaseCoords.getX1(), kDioBaseCoords.getY1() - 20,
                                  kDioBaseCoords.getX1() + (styles::kBaseLength_ * health_percentage), kDioBaseCoords.getY1() - 10));
}

void BattleEngine::RenderAllUnits(const glm::vec2& top_right_corner) const {
  for (const auto& unit_pair : game_state_.GetJojoUnits()) {
    ci::gl::color(ci::Color("white"));
    unit_pair.first.RenderUnit(top_right_corner);
  }
  for (const auto& unit_pair : game_state_.GetDioUnits()) {
    ci::gl::color(ci::Color("white"));
    unit_pair.first.RenderUnit(top_right_corner);
  }
}

}

