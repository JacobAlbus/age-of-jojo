#include "engine/unit.h"

namespace age_of_jojo {

Unit::Unit(UnitType unit_type, float starting_xcoord, bool is_team_jojo) : is_team_jojo_(is_team_jojo) {
  switch(unit_type) {
    case UnitType::Infantry:
      health_ = 100.0f;
      attack_power_ = 20.0f;
      attack_speed_ = 1.0f;
      break;
    case UnitType::Slinger:
      health_ = 100.0f;
      attack_power_ = 10.0f;
      ranged_attack_power_ = 15.0f;
      attack_speed_ = 1.0f;
      break;
    case UnitType::Heavy:
      health_ = 250.0f;
      attack_power_ = 60.0f;
      attack_speed_ = 2.0f;
      break;
  }

  unit_width_ = 50;
  unit_height_ = 100;
  unit_type_ = unit_type;
  position_ = glm::vec2(starting_xcoord, 800);
}

void Unit::RenderUnit(const glm::vec2& top_right_corner) const {
  ci::Rectf unit_hitbox = GetRectHitbox(top_right_corner);
  ci::gl::drawSolidRect(unit_hitbox);

  std::string unit_name = GenerateUnitName();
  glm::vec2 name_position(unit_hitbox.getX1() + (unit_width_ / 2), unit_hitbox.getY1() - 10);

  ci::gl::drawStringCentered(unit_name,
                             name_position,
                             ci::Color("pink"),
                             ci::Font("Helvetica", 20));
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

}
