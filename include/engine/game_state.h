#pragma once

#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_

#include <vector>
#include "engine/unit.h"

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
   * @return all units fighting for Dio
   */
  inline const std::vector<std::pair<Unit, int>>& GetDioUnits() const { return dio_units_; };

  /**
   * @return amount of health left for Dio's base
   */
  inline float GetDioHealth() const { return dio_health_; };

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

  void update_unit_health();

  /**
   * Update positions of all Units
   */
  void UpdatePositions(const glm::vec2& top_right_corner,
                       const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords);

  bool CheckUnitCollisions(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const;


 private:
  std::vector<std::pair<Unit, int>> jojo_units_;
  float jojo_health_;

  std::vector<std::pair<Unit, int>> dio_units_;
  float dio_health_;
};

}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_GAME_STATE_H_
