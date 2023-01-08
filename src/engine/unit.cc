#include "engine/unit.h"
#include "cinder/app/App.h"
#include "engine/styles.h"

namespace age_of_jojo {

Unit::Unit(UnitType unit_type,
           float starting_xcoord,
           bool is_team_jojo) :
           is_team_jojo_(is_team_jojo),
           unit_type_(unit_type),
           timer_(ci::Timer()),
           is_colliding_(false) {

  switch(unit_type) {
    case UnitType::Infantry:
      health_ = 100.0f;
      attack_power_ = 20.0f;
      attack_speed_ = 1.0f;
      unit_width_ = 50;
      unit_height_ = 100;
      sprite_ = GetSprite();
      break;
    case UnitType::Slinger:
      health_ = 100.0f;
      attack_power_ = 10.0f;
      ranged_attack_power_ = 15.0f;
      attack_speed_ = 1.0f;
      unit_width_ = 50;
      unit_height_ = 100;
      sprite_ = GetSprite();
      break;
    case UnitType::Heavy:
      health_ = 250.0f;
      attack_power_ = 60.0f;
      attack_speed_ = 2.0f;
      unit_width_ = 80;
      unit_height_ = 140;
      sprite_ = GetSprite();
      break;
  }

  position_ = glm::vec2(starting_xcoord, styles::kWindowSize_ - unit_height_);
}

void Unit::RenderUnit(const glm::vec2& top_right_corner) const {
//  if (CanAttack()) {
//    ci::gl::color(ci::Color("purple"));
//  } else {
//    ci::gl::color(ci::Color("red"));
//  }
//
//  std::ostringstream sss;
//  sss << "(" << health_ << ")";
//  std::string s2(sss.str());
//
//  if (is_team_jojo_) {
//    ci::gl::drawStringCentered(s2,
//                               glm::vec2(500, 200),
//                               ci::Color("pink"),
//                               ci::Font("Helvetica", 60));
//  } else {
//    ci::gl::drawStringCentered(s2,
//                               glm::vec2(300, 200),
//                               ci::Color("pink"),
//                               ci::Font("Helvetica", 60));
//  }
  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  ci::gl::draw(sprite_, unit_hitbox);

  float margin = 3.0f;

  ci::gl::color(ci::Color("red"));
  ci::gl::drawSolidRect(ci::Rectf(unit_hitbox.getX1() + margin, unit_hitbox.getY1() - 15,
                                  unit_hitbox.getX2() - margin, unit_hitbox.getY1() - 10));

  ci::gl::color(ci::Color("green"));
  float health_percentage = health_ / GetMaxHealth();
  ci::gl::drawSolidRect(ci::Rectf(unit_hitbox.getX1() + margin, unit_hitbox.getY1() - 15,
                                  unit_hitbox.getX1() - margin + (unit_width_ * health_percentage), unit_hitbox.getY1() - 10));
}

void Unit::UpdatePosition(float x_coord_change) {
  if (is_team_jojo_) {
    position_.x += x_coord_change;
  } else {
    position_.x -= x_coord_change;
  }
}

bool Unit::CheckCollision(const ci::Rectf& entity, const glm::vec2& top_right_corner) const {
  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  return (unit_hitbox.getX1() <= entity.getX2() && unit_hitbox.getX2() >= entity.getX1() &&
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

bool Unit::IsAlive() const {
  return health_ > 0;
}

bool Unit::CanAttack() const {
  return timer_.getSeconds() >= attack_speed_;
}

void Unit::UpdateHealth(float attack_damage) {
  health_ -= attack_damage;
}

float Unit::GetMaxHealth() const {
  switch(unit_type_) {
    case UnitType::Infantry:
    case UnitType::Slinger:
      return 100.0f;
    case UnitType::Heavy:
      return 250.0f;
    default:
      return 1.0f;
  }
}

void Unit::ContinueTimer() {
  if (timer_.isStopped()) {
    timer_.start();
  } else {
    timer_.resume();
  }

  if (timer_.getSeconds() > attack_speed_) {
    timer_.stop();
  }
}

void Unit::StopTimer() {
  timer_.stop();
}

}
