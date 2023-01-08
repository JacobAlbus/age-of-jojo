#include "engine/game_state.h"
#include "engine/styles.h"

namespace age_of_jojo {

GameState::GameState() {
  jojo_health_ = 10000.0f;
  dio_health_ = 10000.0f;
}

void GameState::AddDioUnit(UnitType unit_type, const glm::vec2& top_right_corner) {
  float xcoord = styles::kBackgroundLength_ - styles::kBaseLength_;
  Unit new_unit = Unit(unit_type, xcoord, false);

  std::pair<Unit, int> new_pair(new_unit, dio_units_.size());
  dio_units_.emplace_back(new_pair);
}

void GameState::AddJojoUnit(UnitType unit_type, const glm::vec2& top_right_corner) {
  float xcoord = styles::kBaseLength_;
  Unit new_unit = Unit(unit_type, xcoord, true);

  std::pair<Unit, int> new_pair(new_unit, jojo_units_.size());
  jojo_units_.emplace_back(new_pair);
}

void GameState::UpdatePositions(const glm::vec2& top_right_corner,
                                const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords) {
  float unit_speed = 3.0f;

  for (auto& pair : jojo_units_) {
    if (pair.first.CheckCollision(dio_base_coords, top_right_corner) ||
        CheckUnitCollisions(pair, top_right_corner)) {
      continue;
    }
    pair.first.UpdatePosition(unit_speed);
  }

  for (auto& pair : dio_units_) {
    if (pair.first.CheckCollision(jojo_base_coords, top_right_corner) ||
        CheckUnitCollisions(pair, top_right_corner)) {
      continue;
    }
    pair.first.UpdatePosition(unit_speed);
  }
}

void GameState::UpdateBaseHealth(float attack_power, bool is_attack_jojo_base) {
  if (is_attack_jojo_base) {
    jojo_health_ -= attack_power;
  } else {
    dio_health_ -= attack_power;
  }
}

void GameState::UpdateUnitsHealth(const glm::vec2& top_right_corner) {
  if (!jojo_units_.empty() && !dio_units_.empty() &&
      CheckEnemyCollision(jojo_units_[0].first, top_right_corner)) {

    if (jojo_units_[0].first.CanAttack()) {
      dio_units_[0].first.UpdateHealth(jojo_units_[0].first.GetAttackPower());
    }
    if (dio_units_[0].first.CanAttack()) {
      jojo_units_[0].first.UpdateHealth(dio_units_[0].first.GetAttackPower());
    }
  }
}

void GameState::RemoveDeadUnits() {
  std::vector<std::pair<Unit, int>> alive_jojo_units;
  for (const auto& pair : jojo_units_) {
    if (pair.first.IsAlive()) {
      std::pair<Unit, int> alive_unit(pair.first, alive_jojo_units.size());
      alive_jojo_units.emplace_back(alive_unit);
    }
  }

  std::vector<std::pair<Unit, int>> alive_dio_units;
  for (const auto& pair : dio_units_) {
    if (pair.first.IsAlive()) {
      std::pair<Unit, int> alive_unit(pair.first, alive_dio_units.size());
      alive_dio_units.emplace_back(alive_unit);
    }
  }

  jojo_units_ = alive_jojo_units;
  dio_units_ = alive_dio_units;
}

void GameState::UpdateCanUnitAttack(const glm::vec2 &top_right_corner,
                                    const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords) {
  for (auto& pair : jojo_units_) {

    pair.first.is_colliding_ = CheckEnemyCollision(pair.first, top_right_corner);

    if (CheckEnemyCollision(pair.first, top_right_corner) ||
        pair.first.CheckCollision(dio_base_coords, top_right_corner)) {
      pair.first.ContinueTimer();
    } else {
      pair.first.StopTimer();
    }
  }

  for (auto& pair : dio_units_) {
    if (CheckEnemyCollision(pair.first, top_right_corner) ||
        pair.first.CheckCollision(jojo_base_coords, top_right_corner)) {
      pair.first.ContinueTimer();
    } else {
      pair.first.StopTimer();
    }
  }
}

bool GameState::CheckEnemyCollision(const Unit& unit, const glm::vec2& top_right_corner) const {
  if (unit.IsTeamJojo() && !dio_units_.empty()) {
    ci::Rectf dio_unit_hitbox = dio_units_[0].first.GetRectHitbox(top_right_corner);
    return unit.CheckCollision(dio_unit_hitbox, top_right_corner);

  } else if (!unit.IsTeamJojo() && !jojo_units_.empty()) {
    ci::Rectf jojo_unit_hitbox = jojo_units_[0].first.GetRectHitbox(top_right_corner);
    return unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
  }

  return false;
}
bool GameState::CheckAlliedCollision(const std::pair<Unit, int>& unit_pair,
                                     const glm::vec2 &top_right_corner) const {
  bool is_colliding = false;
  Unit unit = unit_pair.first;
  size_t unit_index = unit_pair.second;

  if (unit.IsTeamJojo()) {
    if (unit_index < jojo_units_.size() - 1) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[unit_index + 1].first.GetRectHitbox(top_right_corner);
      is_colliding = unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
    }
    if (unit_index > 0) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
    }

  } else {
    if (unit_index < dio_units_.size() - 1) {
      ci::Rectf dio_unit_hitbox = dio_units_[unit_index + 1].first.GetRectHitbox(top_right_corner);
      is_colliding = unit.CheckCollision(dio_unit_hitbox, top_right_corner);
    }
    if (unit_index > 0) {
      ci::Rectf dio_unit_hitbox = dio_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(dio_unit_hitbox, top_right_corner);
    }
  }

  return is_colliding;
}

bool GameState::CheckUnitCollisions(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const {
  return CheckEnemyCollision(unit_pair.first, top_right_corner) ||
         CheckAlliedCollision(unit_pair, top_right_corner);
}

}