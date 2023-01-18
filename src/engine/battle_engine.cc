#include "engine/battle_engine.h"
#include "engine/player_controller.h"
#include "engine/ai_controller.h"
#include "engine/consts.h"

namespace age_of_jojo {

BattleEngine::BattleEngine() : game_state_(GameState()),
                               player1_controller_(new PlayerController()),
                               player2_controller_(new AIController()),
                               kUnitCosts_(unit_values::GetUnitCosts()) {

  std::vector<std::string> names = {"infantry", "slinger", "heavy", "add_turret", "sell_turret",
                                    "upgrade", "small_turret", "medium_turret", "large_turret"};

  for (const auto& name : names) {
    std::string file_path = "unit image/";
    file_path.append(name);
    file_path.append(".png");
    ci::fs::path path = ci::fs::path(file_path);
    hud_images_.insert(std::make_pair(name, ci::gl::Texture::create(ci::loadImage(cinder::app::loadAsset(path)))));
  }
}

BattleEngine::~BattleEngine() {
  delete player1_controller_;
  delete player2_controller_;
}

BattleEngine::BattleEngine(const BattleEngine &src) : kUnitCosts_(unit_values::GetUnitCosts()) {
  this->game_state_ = src.game_state_;
  BaseController* new_player1 = new PlayerController();
  BaseController* new_player2 = new AIController();
  delete player1_controller_;
  delete player2_controller_;
  this->player1_controller_ = new_player1;
  this->player2_controller_ = new_player2;
  this->hud_images_ = src.hud_images_;
}

BattleEngine& BattleEngine::operator=(const BattleEngine& rhs) {
  if (this == &rhs) {
    return *this;
  }

  this->game_state_ = rhs.game_state_;
  BaseController* new_player1 = new PlayerController();
  BaseController* new_player2 = new AIController();
  delete player1_controller_;
  delete player2_controller_;
  this->player1_controller_ = new_player1;
  this->player2_controller_ = new_player2;
  this->hud_images_ = rhs.hud_images_;

  return *this;
}

void BattleEngine::RestartGame() {
  game_state_ = GameState();
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

  game_state_.UpdateCanUnitAttack(top_right_corner, kJojoBaseCoords, kDioBaseCoords);
  RemoveDeadUnits();
  HandlePlayer1Input(top_right_corner);
  HandlePlayer2Input(top_right_corner);
  UpdateUnitPositions(top_right_corner);
  game_state_.UpdateUnitQueue();

  UpdateBaseHealth(top_right_corner); // Health related checks have to go first
  UpdateUnitsHealth(top_right_corner);

  game_state_.ResetMouse(); // Must be at end
}

void BattleEngine::HandlePlayer1Input(const glm::vec2& top_right_corner) {
  ControllerAction player1_action = player1_controller_->CalculateCurrentAction(game_state_);

  if (CannotBuildUnit(player1_action, true)) {
    return;
  }

  switch (player1_action) {
    case ControllerAction::kSendInfantry:
      game_state_.AddJojoUnit(UnitType::Infantry, top_right_corner);
      game_state_.UpdateJojoMoney(-kUnitCosts_.at(game_state_.GetJojoEra()).at(Infantry));
      break;
    case ControllerAction::kSendSlinger:
      game_state_.AddJojoUnit(UnitType::Slinger, top_right_corner);
      game_state_.UpdateJojoMoney(-kUnitCosts_.at(game_state_.GetJojoEra()).at(Slinger));
      break;
    case ControllerAction::kSendHeavy:
      game_state_.AddJojoUnit(UnitType::Heavy, top_right_corner);
      game_state_.UpdateJojoMoney(-kUnitCosts_.at(game_state_.GetJojoEra()).at(Heavy));
      break;
    case ControllerAction::kUpgrade:
      bool is_team_jojo = true;
      game_state_.UpgradeEra(is_team_jojo);
  }
}

void BattleEngine::HandleKeyDown(const ci::app::KeyEvent& event, const glm::vec2& top_right_corner) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_i:
      game_state_.AddDioUnit(UnitType::Infantry, top_right_corner);
      break;
    case ci::app::KeyEvent::KEY_s:
      game_state_.AddDioUnit(UnitType::Slinger, top_right_corner);
      break;
    case ci::app::KeyEvent::KEY_h:
      game_state_.AddDioUnit(UnitType::Heavy, top_right_corner);
      break;
  }
//  game_state_.UpdateDioMoney(-15);
}

void BattleEngine::HandlePlayer2Input(const glm::vec2& top_right_corner) {
//  ControllerAction player2_action = player2_controller_->CalculateCurrentAction(game_state_);
//  if (CannotBuildUnit(player2_action, false)) {
//    return;
//  }
//  switch (player2_action) {
//    case ControllerAction::kSendInfantry:
//      game_state_.AddDioUnit(UnitType::Infantry, top_right_corner);
//      game_state_.UpdateDioMoney(-15);
//      break;
//    case ControllerAction::kSendSlinger:
//      game_state_.AddDioUnit(UnitType::Slinger, top_right_corner);
//      game_state_.UpdateDioMoney(-25);
//      break;
//    case ControllerAction::kSendHeavy:
//      game_state_.AddDioUnit(UnitType::Heavy, top_right_corner);
//      game_state_.UpdateDioMoney(-100);
//      break;
//
//  }
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
  game_state_.UpdateBaseHealth(top_right_corner);
}

void BattleEngine::UpdateUnitsHealth(const glm::vec2& top_right_corner) {
  game_state_.UpdateUnitsHealth(top_right_corner);
}

void BattleEngine::RemoveDeadUnits() {
  game_state_.RemoveDeadUnits();
}

void BattleEngine::HandleMouseClick(const ci::app::MouseEvent& event) {
  game_state_.HandleMouseClick(event);
}

bool BattleEngine::CannotBuildUnit(ControllerAction player_action, bool is_team_jojo) const {
  UnitType unit_type;
  switch (player_action) {
    case ControllerAction::kSendInfantry:
      unit_type = Infantry;
      break;
    case ControllerAction::kSendSlinger:
      unit_type = Slinger;
      break;
    case ControllerAction::kSendHeavy:
      unit_type = Heavy;
      break;
    default:
      return false;
  }

  if (is_team_jojo) {
    return (game_state_.GetJojoQueue().size() >= game_values::kMaxQueueLength_ ||
            kUnitCosts_.at(game_state_.GetJojoEra()).at(unit_type) > game_state_.GetJojoMoney());
  } else {
    return (game_state_.GetDioQueue().size() >= game_values::kMaxQueueLength_ ||
            kUnitCosts_.at(game_state_.GetJojoEra()).at(unit_type) < game_state_.GetDioMoney());
  }
}

GameMode BattleEngine::GetGameMode() const {
  if (game_state_.GetDioHealth() <= 0) {
    return GameMode::kJojoWins_;
  } else if (game_state_.GetJojoHealth() <= 0) {
    return GameMode::kDioWins_;
  } else {
    return GameMode::kPlaying_;
  }
}


bool BattleEngine::IsMouseInsideImage(const ci::Rectf& image_hitbox, const glm::vec2& mouse_pos) {
  return (mouse_pos.x > image_hitbox.getX1() && mouse_pos.x < image_hitbox.getX2() &&
          mouse_pos.y > image_hitbox.getY1() && mouse_pos.y < image_hitbox.getY2());
}

void BattleEngine::RenderUnitCost(const glm::vec2& mouse_pos) const {
  glm::vec2 render_position(styles::kWindowSize_ / 2.5f, game_values::kQueueSlotSize_);
  std::stringstream message;
  message << "Cost: ";

  if (IsMouseInsideImage(game_values::kInfantryButtonPos_, mouse_pos)) {
    message << kUnitCosts_.at(game_state_.GetJojoEra()).at(Infantry);
  } else if (IsMouseInsideImage(game_values::kSlingerButtonPos_, mouse_pos)) {
    message << kUnitCosts_.at(game_state_.GetJojoEra()).at(Slinger);
  } else if (IsMouseInsideImage(game_values::kHeavyButtonPos_, mouse_pos)) {
    message << kUnitCosts_.at(game_state_.GetJojoEra()).at(Heavy);
  }

  ci::gl::drawString(message.str(),
                     render_position,
                     ci::Color("black"),
                     ci::Font("Helvetica", 30));
}

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
  float health_percentage = game_state_.GetJojoHealth() / GetMaxBaseHealth(game_state_.GetJojoEra());
  ci::gl::drawSolidRect(ci::Rectf(kJojoBaseCoords.getX1(), kJojoBaseCoords.getY1() - 20,
                                  kJojoBaseCoords.getX1() + (styles::kBaseLength_ * health_percentage), kJojoBaseCoords.getY1() - 10));

  // Render Jojo health
  std::stringstream jojo_health;
  jojo_health << game_state_.GetJojoHealth();
  ci::gl::drawStringCentered(jojo_health.str(), kJojoBaseCoords.getUpperRight(),
                             ci::Color("black"), ci::Font("Helvetica", 35));

  // Draw Dio health bar
  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);

  ci::gl::color(ci::Color("red"));
  ci::gl::drawSolidRect(ci::Rectf(kDioBaseCoords.getX1(), kDioBaseCoords.getY1() - 20,
                                  kDioBaseCoords.getX2(), kDioBaseCoords.getY1() - 10));

  ci::gl::color(ci::Color("green"));
  health_percentage = game_state_.GetDioHealth() / GetMaxBaseHealth(game_state_.GetDioEra());
  ci::gl::drawSolidRect(ci::Rectf(kDioBaseCoords.getX1(), kDioBaseCoords.getY1() - 20,
                                  kDioBaseCoords.getX1() + (styles::kBaseLength_ * health_percentage), kDioBaseCoords.getY1() - 10));

  // Render Dio health
  std::stringstream dio_health;
  dio_health << game_state_.GetDioHealth();
  ci::gl::drawStringCentered(dio_health.str(), kDioBaseCoords.getUpperLeft(),
                             ci::Color("black"), ci::Font("Helvetica", 35));
}

void BattleEngine::RenderPlayer1HUD() const {
  ci::gl::color(ci::Color("white"));
  ci::gl::draw(hud_images_.at("infantry"), game_values::kInfantryButtonPos_);
  ci::gl::draw(hud_images_.at("slinger"), game_values::kSlingerButtonPos_);
  ci::gl::draw(hud_images_.at("heavy"), game_values::kHeavyButtonPos_);

  ci::gl::draw(hud_images_.at("small_turret"), game_values::kSmallTurretButtonPos_);
  ci::gl::draw(hud_images_.at("medium_turret"), game_values::kMediumTurretButtonPos_);
  ci::gl::draw(hud_images_.at("large_turret"), game_values::kLargeTurretButtonPos_);

  ci::gl::draw(hud_images_.at("sell_turret"), game_values::kSellTurretButtonPos_);
  ci::gl::draw(hud_images_.at("add_turret"), game_values::kAddTurretButtonPos_);
  ci::gl::draw(hud_images_.at("upgrade"), game_values::kUpgradeButtonPos_);

  std::ostringstream money_stream;
  money_stream << "Money: " << game_state_.GetJojoMoney();
  std::string money(money_stream.str());
  ci::gl::drawString(money,
                     glm::vec2(30, 240),
                     ci::Color("black"),
                     ci::Font("Helvetica", 30));

  std::ostringstream experience_stream;
  experience_stream << "Experience: " << game_state_.GetJojoExperience();
  std::string experience(experience_stream.str());
  ci::gl::drawString(experience,
                     glm::vec2(30, 260),
                     ci::Color("black"),
                     ci::Font("Helvetica", 30));
}

void BattleEngine::RenderPlayer1Queue(const glm::vec2 &top_right_corner) const {
  std::vector<Unit> queue = game_state_.GetJojoQueue();
  size_t index = 0;
  for (; index < queue.size(); index++) {
    ci::gl::color(ci::Color("red"));
    float offset = ((game_values::kQueueSlotSize_ * 1.5f) * index);
    ci::gl::drawSolidRect(ci::Rectf(game_values::kQueueSlot1_.getX1() - offset,
                                    game_values::kQueueSlot1_.getY1(),
                                    game_values::kQueueSlot1_.getX2() - offset,
                                    game_values::kQueueSlot1_.getY2()));
  }

  for (size_t j = index; j < game_values::kMaxQueueLength_; j++) {
    ci::gl::color(ci::Color("black"));
    float offset = ((game_values::kQueueSlotSize_ * 1.5f) * j);
    ci::gl::drawSolidRect(ci::Rectf(game_values::kQueueSlot1_.getX1() - offset,
                                    game_values::kQueueSlot1_.getY1(),
                                    game_values::kQueueSlot1_.getX2() - offset,
                                    game_values::kQueueSlot1_.getY2()));
  }

  // draw queue loading bar
  ci::gl::color(ci::Color("black"));
  float x1 = styles::kWindowSize_ / 2.5f;
  float x2 = styles::kWindowSize_ * (2.0f / 2.5f);
  float y = 2 * game_values::kQueueSlotSize_;

  ci::gl::drawSolidRect(ci::Rectf(x1, y,
                                  x2, y + 2));
  ci::gl::drawSolidRect(ci::Rectf(x2, y,
                                  x2 + 2, y + 20));

  ci::gl::drawSolidRect(ci::Rectf(x1, y + 18,
                                  x2, y + 20));
  ci::gl::drawSolidRect(ci::Rectf(x1, y,
                                  x1 + 2, y + 20));

  ci::gl::color(ci::Color("red"));
  if (!queue.empty()) {
    float queue_completion = queue[0].CompletionPercent();
    ci::gl::drawSolidRect(ci::Rectf(x1, y,
                                    x1 + (x1 * queue_completion), y + 20));
  }

  // draw queued unit name
  if (!queue.empty()) {
    ci::gl::color(ci::Color("black"));
    std::string name = queue[0].GenerateUnitName();
    ci::gl::drawString(name, glm::vec2(x1, y + 25), ci::Color("black"), ci::Font("Helvetica", 35));
  }
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

float BattleEngine::GetMaxBaseHealth(Era era) const {
  return player_values::GetEraHealth().at(era);
}


}

