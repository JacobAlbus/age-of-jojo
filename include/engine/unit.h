#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_

#include <string>
#include "cinder/gl/gl.h"
#include "cinder/Timer.h"

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
   * Renders unit
   */
  void RenderUnit(const glm::vec2& top_right_corner) const;

  /**
   * Updates position of unit
   * @param x_coord_change how much their x coordinate changes
   */
  void UpdatePosition(float x_coord_change);

  /**
   * Check if unit is alive
   * @return boolean if unit is alive
   */
  bool IsAlive() const;

  inline float GetAttackPower() const { return attack_power_; };

  inline bool IsTeamJojo() const { return is_team_jojo_; };

  inline void SetHealth(float health) { health_ = health; };

  ci::Rectf GetRectHitbox(const glm::vec2& top_right_corner) const;

  bool CheckCollision(const ci::Rectf& entity, const glm::vec2& top_right_corner) const;

  bool CanAttack() const;

  void ContinueTimer();

  void StopTimer();

  /**
   * Update health of unit
   * @param attack_damage how much damage unit sustained
   */
  void UpdateHealth(float attack_damage);

  bool is_colliding_;

 private:

  std::string GenerateUnitName() const;

  ci::gl::Texture2dRef GetSprite() const;

  float GetMaxHealth() const;

  ci::Timer timer_;

  std::string name_;
  bool is_team_jojo_;
  ci::gl::TextureRef sprite_;
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
