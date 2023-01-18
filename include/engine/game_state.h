#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_

#include <vector>
#include "engine/unit.h"
#include "cinder/app/App.h"

namespace age_of_jojo {

class GameState {
 public:

  GameState();

  /**
   * @return all units fighting for Jojo
   */
  inline const std::vector<std::pair<Unit, int>>& GetJojoUnits() const {return jojo_units_; };

  /**
   * @return amount of health left for Jojo's base
   */
  inline float GetJojoHealth() const { return jojo_health_; };

  /**
   * @return amount of jojo money
   */
  inline int GetJojoMoney() const { return jojo_money_; };

  /**
   * Update amount of money jojo has
   */
  inline void UpdateJojoMoney(int money_change) { jojo_money_ += money_change; };

  /**
   * @return amount of jojo experience
   */
  inline int GetJojoExperience() const { return jojo_experience_; };

  /**
   * @return Era jojo is in
   */
  inline Era GetJojoEra() const { return jojo_era_; };

  /**
   * @return Units in Jojo's queue
   */
  inline const std::vector<Unit>& GetJojoQueue() const { return jojo_unit_queue_; };

  /**
   * Update amount of experience jojo has
   */
  inline void UpdateJojoExperience(int experience_change) { jojo_experience_ += experience_change; };

  /**
   * @return all units fighting for Dio
   */
  inline const std::vector<std::pair<Unit, int>>& GetDioUnits() const { return dio_units_; };

  /**
   * @return amount of health left for Dio's base
   */
  inline float GetDioHealth() const { return dio_health_; };

  /**
   * @return amount of dio money
   */
  inline int GetDioMoney() const { return dio_money_; };

  /**
   * Update amount of money dio has
   */
  inline void UpdateDioMoney(int money_change) { dio_money_ += money_change; };

  /**
   * @return amount of dio experience
   */
  inline int GetDioExperience() const { return dio_experience_; };

  /**
   * Update amount of experience dio has
   */
  inline void UpdateDioExperience(int experience_change) { dio_experience_ += experience_change; };

  /**
   * @return Era dio is in
   */
  inline Era GetDioEra() const { return dio_era_; };

  /**
   * @return Units in Dio's queue
   */
  inline const std::vector<Unit>& GetDioQueue() const { return dio_unit_queue_; };

  /**
   * Adds new unit to Dio's army
   * @param UnitType type of unit to be added
   */
  void AddDioUnit(UnitType unit_type, const glm::vec2& top_right_corner);

  /**
   * Adds new unit to Jojo's army
   * @param UnitType type of unit to be added
   */
  void AddJojoUnit(UnitType unit_type, const glm::vec2& top_right_corner);

  /**
   * Update positions of all Units
   */
  void UpdatePositions(const glm::vec2& top_right_corner,
                       const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords);

  /**
   * Checks for and removes dead players, maintains order of units
   */
  void RemoveDeadUnits();

  /**
   * Update healths of units from attacks
   * @param top_right_corner coordinates of top right corner of map
   */
  void UpdateUnitsHealth(const glm::vec2& top_right_corner);

  /**
   * Updates health of base depending on attack power
   * @param attack_power how much damage will be inflicted on base
   * @param is_attack_jojo_base is the attack on jojo or dio base
   */
  void UpdateBaseHealth(const glm::vec2& top_right_corner);

  /**
   * Checks if each unit is able to attack
   * @param top_right_corner top right corner of map
   * @param jojo_base_coords coordinates of jojo base
   * @param dio_base_coords coorindates of dio base
   */
  void UpdateCanUnitAttack(const glm::vec2& top_right_corner,
                           const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords);

  /**
   * Handles mouse click
   * @param event data on mouse placement
   */
  void HandleMouseClick(const ci::app::MouseEvent& event);

  /**
   * Sets mouse position to be in pre-defined "forbidden" position
   */
  void ResetMouse();

  const ci::app::MouseEvent& GetMouse() const { return mouse_event_; };

  void UpdateUnitQueue();

  bool CheckEnemyCollision(const Unit& unit, const glm::vec2& top_right_corner) const;

  void UpgradeEra(bool is_team_jojo);

 private:

  void UpgradeBaseHealth(bool is_team_jojo);

  bool CheckEnemeyInRange(const Unit& unit, const glm::vec2& top_right_corner, const ci::Rectf& entity_hitbox) const;

  bool CheckAlliedCollision(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const;

  bool CheckUnitCollisions(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const;

  std::vector<std::pair<Unit, int>> jojo_units_;
  std::vector<Unit> jojo_unit_queue_;
  float jojo_health_;
  int jojo_money_;
  int jojo_experience_;
  Era jojo_era_;

  std::vector<std::pair<Unit, int>> dio_units_;
  std::vector<Unit> dio_unit_queue_;
  float dio_health_;
  int dio_money_;
  int dio_experience_;
  Era dio_era_;

  ci::app::MouseEvent mouse_event_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_
