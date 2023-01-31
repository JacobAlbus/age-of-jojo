#include "engine/unit.h"
#include "cinder/app/App.h"
#include "engine/consts.h"
#include "engine/game_state.h"

namespace age_of_jojo {

Unit::Unit(UnitType unit_type,
           float starting_xcoord,
           bool is_team_jojo,
           Era unit_era) :
           is_team_jojo_(is_team_jojo),
           unit_type_(unit_type),
           attack_timer_(ci::Timer()),
           deployment_timer_(ci::Timer()),
           unit_era_(unit_era) {

  attack_time_ = 0.0;
  ranged_attack_time_ = 0.0;

  std::unordered_map<Era, std::unordered_map<UnitType, int>> unit_costs = unit_values::GetUnitCosts();
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_health = unit_values::GetUnitHealth();
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_attack_power = unit_values::GetUnitAttackPower();
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_deployment_time = unit_values::GetUnitDeploymentTime();

  switch(unit_type) {
    case UnitType::Infantry:
      attack_speed_ = unit_values::kAttackSpeed_;
      attack_range_ = 0.0f;
      unit_width_ = 50;
      unit_height_ = 100;
      sprite_ = GetSprite();
      break;
    case UnitType::Slinger:
      attack_range_ = 300.0f;
      attack_speed_ = unit_values::kAttackSpeed_;
      unit_width_ = 50;
      unit_height_ = 100;
      sprite_ = GetSprite();
      break;
    case UnitType::Heavy:
      attack_speed_ = unit_values::kAttackSpeed_;
      attack_range_ = 0.0f;
      unit_width_ = 80;
      unit_height_ = 140;
      sprite_ = GetSprite();
      break;
  }

  ci::audio::SourceFileRef attack_sound_source = ci::audio::load(ci::app::loadAsset("audio/attacks/bang.mp3"));

  health_ = unit_health.at(unit_era).at(unit_type);
  attack_power_ = unit_attack_power.at(unit_era).at(unit_type);
  cost_ = unit_costs.at(unit_era).at(unit_type_);
  deployment_time_ = unit_deployment_time.at(unit_era).at(unit_type) / styles::kGameSpeed_;
  ranged_attack_speed_ = 0.8f;

  attack_sound_ = ci::audio::Voice::create(attack_sound_source);
  attack_sound_->setVolume(0.0f);
  position_ = glm::vec2(starting_xcoord, styles::kWindowHeight_ - unit_height_);
}

void Unit::RenderUnit(const glm::vec2& top_right_corner, const ci::gl::BatchRef health_bar) const {
//  if (is_team_jojo_) {
//    std::stringstream message;
//    message << attack_timer_.getSeconds() << "       " << ranged_attack_timer_.getSeconds();
//
//    ci::gl::drawString(message.str(),
//                       glm::vec2(300, 300),
//                       ci::Color("pink"),
//                       ci::Font("Helvetica", 30));
//  } else {
//    std::stringstream message;
//    message << attack_timer_.getSeconds() << "       " << ranged_attack_timer_.getSeconds();
//
//    ci::gl::drawString(message.str(),
//                       glm::vec2(600, 300),
//                       ci::Color("black"),
//                       ci::Font("Helvetica", 30));
//  }
  float margin = 3.0f;

  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  ci::gl::draw(sprite_, unit_hitbox);

  ci::gl::pushModelMatrix();
  ci::gl::translate(unit_hitbox.getUpperLeft());
  ci::gl::scale(glm::vec2(unit_width_ - margin, 10));
  ci::gl::color(ci::Color(ci::CM_HSV, 1, 0, 0));
  health_bar->draw();
  ci::gl::popModelMatrix();


  float health_percentage = std::min(1.0f, (float) health_ / (float) GetMaxHealth());
  ci::gl::pushModelMatrix();
  ci::gl::translate(unit_hitbox.getUpperLeft());
  ci::gl::scale(glm::vec2((unit_width_ - margin) * (health_percentage), 10));
  ci::gl::color(ci::Color(ci::CM_HSV, 1, 1, 1));
  health_bar->draw();
  ci::gl::popModelMatrix();
//  std::stringstream part;
//  part << static_cast<int>(unit_era_);
//  ci::gl::drawStringCentered(part.str(), unit_hitbox.getUpperRight(),
//                             ci::Color("black"), ci::Font("Helvetica", 25));

//
//  ci::gl::color(ci::Color("red"));
//  ci::gl::drawSolidRect(ci::Rectf(unit_hitbox.getX1() + margin, unit_hitbox.getY1() - 15,
//                                  unit_hitbox.getX2() - margin, unit_hitbox.getY1() - 10));
//
//  ci::gl::color(ci::Color("green"));
//  float health_percentage = (float) health_ / (float) GetMaxHealth();
//  ci::gl::drawSolidRect(ci::Rectf(unit_hitbox.getX1() + margin, unit_hitbox.getY1() - 15,
//                                  unit_hitbox.getX1() - margin + (unit_width_ * health_percentage), unit_hitbox.getY1() - 10));
}

void Unit::UpdatePosition(float x_coord_change) {
  attack_time_ = attack_timer_.getSeconds();
  ranged_attack_time_ = ranged_attack_timer_.getSeconds();

  if (is_team_jojo_) {
    position_.x += x_coord_change;
  } else {
    position_.x -= x_coord_change;
  }
}

float Unit::DistnaceToEntity(const ci::Rectf &entity, const glm::vec2& top_right_corner) const {
  ci::Rectf hit_box = GetRectHitbox(top_right_corner);

  if (is_team_jojo_) {
    return std::abs(hit_box.getX2() - entity.getX1());
  } else {
    return std::abs(hit_box.getX1() - entity.getX2());
  }
}

bool Unit::CheckCollision(const ci::Rectf& entity, const glm::vec2& top_right_corner) const {
  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  return (unit_hitbox.getX1() <= entity.getX2() && unit_hitbox.getX2() >= entity.getX1() &&
          unit_hitbox.getY1() <= entity.getY2() && unit_hitbox.getY2() >= entity.getY1());
//  return DistnaceToEntity(entity, top_right_corner) <= 2.0f;
}

bool Unit::CheckInRange(const ci::Rectf &entity,
                        const glm::vec2 &top_right_corner) const {
  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  float x1_range = unit_hitbox.getX1();
  float x2_range = unit_hitbox.getX2();

  if (is_team_jojo_) {
    x2_range += attack_range_;
  } else {
    x1_range -= attack_range_;
  }

  return (x1_range <= entity.getX2() && x2_range >= entity.getX1() &&
          unit_hitbox.getY1() <= entity.getY2() && unit_hitbox.getY2() >= entity.getY1());
}

ci::Rectf Unit::GetRectHitbox(const glm::vec2& top_right_corner) const {
  return  {position_.x + top_right_corner.x,
           position_.y,
           position_.x + unit_width_ + top_right_corner.x,
           position_.y + unit_height_};
}

std::string Unit::GenerateUnitName() const {
  std::string unit_name;

  if (is_team_jojo_) {
    unit_name.append("Jojo's");
  } else {
    unit_name.append("Dio's");
  }

  switch (unit_type_) {
    case UnitType::Infantry:
      unit_name.append(" I");
      break;
    case Slinger:
      unit_name.append(" S");
      break;
    case Heavy:
      unit_name.append(" H");
      break;
  }

  return unit_name;
}

float Unit::CompletionPercent() const {
  return std::min((float) deployment_timer_.getSeconds() / deployment_time_, 1.0f);
}

ci::gl::Texture2dRef Unit::GetSprite() const {
  std::string file_path;

  switch(unit_type_) {
    case UnitType::Infantry:
      file_path = "units/josuke";
      break;
    case UnitType::Slinger:
      file_path = "units/kira";
      break;
    case UnitType::Heavy:
      file_path = "units/dio";
      break;
  }

  if (is_team_jojo_) {
    file_path.append(".png");
  } else {
    file_path.append("_flipped.png");
  }

  ci::fs::path path = ci::fs::path(file_path);
  return ci::gl::Texture::create(ci::loadImage(cinder::app::loadAsset(path)));
}

void Unit::StartDeploymentTimer() {
  if (deployment_timer_.isStopped()) {
    deployment_timer_.start();
  } else {
    deployment_timer_.resume();
  }
}

bool Unit::IsAlive() const {
  return health_ > 0;
}

bool Unit::CanAttack() const {
  return ((attack_timer_.getSeconds() >= attack_speed_ && !attack_timer_.isStopped()) ||
          (ranged_attack_timer_.getSeconds() >= ranged_attack_speed_ && !ranged_attack_timer_.isStopped()));
}

void Unit::UpdateHealth(float attack_damage) {
  health_ -= attack_damage;
}

void Unit::PlayAttackSound() const {
  attack_sound_->start();
}

float Unit::GetMaxHealth() const {
  return unit_values::GetUnitHealth().at(unit_era_).at(unit_type_);
}

bool Unit::IsDeploymentReady() {
  if (deployment_timer_.getSeconds() > deployment_time_) {
    deployment_timer_.stop();
    return true;
  } else {
    return false;
  }
}

void Unit::ContinueTimer() {
  if (attack_timer_.isStopped()) {
    attack_timer_.start();
  } else {
    attack_timer_.resume();
  }
}

void Unit::StopTimer() {
  attack_timer_.stop();
//  attack_timer_.start(); // resets attack timer to 0 seconds
//  attack_timer_.stop();
}

void Unit::ContinueRangeAttackTimer() {
  if (ranged_attack_timer_.isStopped()) {
    ranged_attack_timer_.start();
    attack_power_ *= 0.5f;
  } else {
    ranged_attack_timer_.resume();
  }
}

void Unit::StopRangeAttackTimer() {
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_attack_power = unit_values::GetUnitAttackPower();
  attack_power_ = unit_attack_power.at(unit_era_).at(unit_type_);
//  ranged_attack_timer_.stop();
//  ranged_attack_timer_.start(); // resets attack timer to 0 seconds
  ranged_attack_timer_.stop();

}
nlohmann::json Unit::GetJSON() const {
  nlohmann::json unit_json;

  unit_json["unit_type"] = unit_type_;
  unit_json["health"] = health_;
  unit_json["era"] = unit_era_;

  return unit_json;
}

}
