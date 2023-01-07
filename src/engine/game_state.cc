#include "engine/game_state.h"
#include "engine/styles.h"

namespace age_of_jojo {

GameState::GameState() {
  jojo_health_ = 100.0f;
  dio_health_ = 100.0f;
}

void GameState::AddDioUnit(UnitType unit_type, const glm::vec2& top_right_corner) {
  float xcoord = styles::kBackgroundLength_ - styles::kBaseiLength_;
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

bool GameState::CheckUnitCollisions(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const {
  bool is_colliding = false;
  Unit unit = unit_pair.first;
  size_t unit_index = unit_pair.second;

  if (unit.IsTeamJojo()) {
    if (!dio_units_.empty()) {
      ci::Rectf dio_unit_hitbox = dio_units_[0].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(dio_unit_hitbox, top_right_corner);
    }
    if (unit_index < jojo_units_.size() - 1) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[unit_index + 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
    }
    if (unit_index > 0) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
    }

  } else {
    if (!jojo_units_.empty()) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[0].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
    }
    if (unit_index < dio_units_.size() - 1) {
      ci::Rectf dio_unit_hitbox = dio_units_[unit_index + 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(dio_unit_hitbox, top_right_corner);
    }
    if (unit_index > 0) {
      ci::Rectf dio_unit_hitbox = dio_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      is_colliding = is_colliding || unit.CheckCollision(dio_unit_hitbox, top_right_corner);
    }
  }

  return is_colliding;
}

}