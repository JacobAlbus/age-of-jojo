#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_

#include <string>
#include "cinder/gl/gl.h"
#include "cinder/Timer.h"
#include "engine/consts.h"
#include "cinder/audio/Voice.h"
#include "nlohmann/json.hpp"

namespace age_of_jojo {

class Unit {
 public:
  Unit(UnitType unit_type, float starting_xcoord, bool is_team_jojo, Era unit_era);

  /**
   * Renders unit
   */
  void RenderUnit(const glm::vec2& top_right_corner, const ci::gl::BatchRef health_bar) const;

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

  /**
   * Gets unit's attack power
   */
  inline float GetAttackPower() const { return attack_power_; };

  /**
   * Whether or not unit belongs to team Jojo
   */
  inline bool IsTeamJojo() const { return is_team_jojo_; };

  inline UnitType GetUnitType() const { return unit_type_; };

  /**
   * How much it costs to produce unit
   */
  inline size_t GetCost() const { return cost_; };

  /**
   * Calculate hit box of unit
   * @param top_right_corner top right corner of map
   * @return hit box of unit
   */
  ci::Rectf GetRectHitbox(const glm::vec2& top_right_corner) const;

  /**
   * Checks collision with entity defined by hitbox
   * @param entity hitbox of entity
   * @param top_right_corner top right corner of map
   * @return whether or not the unit is hitting the entity
   */
  bool CheckCollision(const ci::Rectf& entity, const glm::vec2& top_right_corner) const;

  /**
   * Checks if entity is within range of unit
   * @param entity hitbox of entity
   * @param top_right_corner top right corner of map
   * @return whether or not the unit is hitting the entity
   */
  bool CheckInRange(const ci::Rectf& entity, const glm::vec2& top_right_corner) const;

  float DistnaceToEntity(const ci::Rectf& entity, const glm::vec2& top_right_corner) const;

  /**
   * Checks if unit can attack based on timer
   * @return whether or not unit can attack
   */
  bool CanAttack() const;

  /**
   * Continue/start attack timer
   */
  void ContinueTimer();

  /**
   * Stop attack timer
   */
  void StopTimer();

  /**
   * Continue/start attack timer
   */
  void ContinueRangeAttackTimer();

  /**
   * Stop attack timer
   */
  void StopRangeAttackTimer();

  /**
   * Update health of unit
   * @param attack_damage how much damage unit sustained
   */
  void UpdateHealth(float attack_damage);

  void PlayAttackSound() const;

  bool IsDeploymentReady();

  /**
  * Get sprite of unit based on type and era
  * @return sprite of unit
  */
  ci::gl::Texture2dRef GetSprite() const;

  float CompletionPercent() const;

  std::string GenerateUnitName() const;

  void StartDeploymentTimer();

  nlohmann::json GetJSON() const;

 private:


  /**
   * Get max health of unit based on type and era
   * @return max health of unit
   */
  float GetMaxHealth() const;

  ci::Timer attack_timer_;
  ci::Timer ranged_attack_timer_;
  ci::Timer deployment_timer_;
  ci::audio::VoiceRef attack_sound_;

  bool is_team_jojo_;
  ci::gl::TextureRef sprite_;
  float health_;
  float attack_power_;
  float deployment_time_;
  float attack_range_;
  float ranged_attack_speed_;
  size_t cost_;
  float attack_speed_;
  float unit_width_;
  float unit_height_;
  UnitType unit_type_;
  Era unit_era_;
  glm::vec2 position_;

  double attack_time_;
  double ranged_attack_time_;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_UNIT_H_
