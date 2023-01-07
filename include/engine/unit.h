#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_

#include <string>
#include "cinder/gl/gl.h"

namespace age_of_jojo {

enum UnitType {
  Infantry,
  Slinger,
  Heavy
};

class Unit {
 public:
  Unit(UnitType unit_type, float starting_xcoord, bool is_team_jojo);

  /**
   * Update health of unit
   * @param damage_amount how much damage unit sustained
   */
  void update_health(float damage_amount);

  /**
   * Renders unit
   */
  void RenderUnit(const glm::vec2& top_right_corner) const;

  /**
   * Updates position of unit
   * @param x_coord_change how much their x coordinate changes
   */
  void UpdatePosition(float x_coord_change);

  /**
   * Check if unit is dead
   * @return boolean if unit is dead
   */
  bool is_dead() const;

  inline bool IsTeamJojo() const { return is_team_jojo_; };

  ci::Rectf GetRectHitbox(const glm::vec2& top_right_corner) const;

  bool CheckCollision(const ci::Rectf& entity, const glm::vec2& top_right_corner) const;

 private:

  std::string GenerateUnitName() const;

  std::string name_;
  bool is_team_jojo_;
  float health_;
  float attack_power_;
  float ranged_attack_power_;
  float attack_speed_;
  float unit_width_;
  float unit_height_;
  UnitType unit_type_;
  glm::vec2 position_;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_
