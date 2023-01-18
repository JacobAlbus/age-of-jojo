#include "engine/game_state.h"
#include "engine/consts.h"

namespace age_of_jojo {

GameState::GameState() : jojo_health_(player_values::kAge1Health_),
                         jojo_money_(player_values::kStartingMoney_),
                         jojo_experience_(0),
                         jojo_era_(kPart1),
                         dio_health_(player_values::kAge1Health_),
                         dio_money_(player_values::kStartingMoney_),
                         dio_era_(kPart1) {
  mouse_event_.setPos(game_values::kForbiddenMousePos_);
}

void GameState::HandleMouseClick(const ci::app::MouseEvent& event) {
  mouse_event_.setPos(event.getPos());
}

void GameState::ResetMouse() {
  mouse_event_.setPos(game_values::kForbiddenMousePos_);
}

void GameState::AddDioUnit(UnitType unit_type, const glm::vec2& top_right_corner) {
  float xcoord = styles::kBackgroundLength_ - styles::kBaseLength_;
  Unit new_unit = Unit(unit_type, xcoord, false, dio_era_);

  dio_unit_queue_.emplace_back(new_unit);
}

void GameState::AddJojoUnit(UnitType unit_type, const glm::vec2& top_right_corner) {
  float xcoord = styles::kBaseLength_;
  Unit new_unit = Unit(unit_type, xcoord, true, jojo_era_);

  jojo_unit_queue_.emplace_back(new_unit);
}

void GameState::UpdateUnitQueue() {
  if (!jojo_unit_queue_.empty() && jojo_unit_queue_[0].IsDeploymentReady()) {
    std::pair<Unit, int> new_pair(jojo_unit_queue_[0], jojo_units_.size());
    jojo_units_.emplace_back(new_pair);
    jojo_unit_queue_.erase(jojo_unit_queue_.begin());
  }
  if (!jojo_unit_queue_.empty()) {
    jojo_unit_queue_[0].StartDeploymentTimer();
  }

  if (!dio_unit_queue_.empty() && dio_unit_queue_[0].IsDeploymentReady()) {
    std::pair<Unit, int> new_pair(dio_unit_queue_[0], dio_units_.size());
    dio_units_.emplace_back(new_pair);
    dio_unit_queue_.erase(dio_unit_queue_.begin());
  }
  if (!dio_unit_queue_.empty()) {
    dio_unit_queue_[0].StartDeploymentTimer();
  }
}

void GameState::UpdatePositions(const glm::vec2& top_right_corner,
                                const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords) {
  float unit_speed = 1.0f;

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

void GameState::UpdateBaseHealth(const glm::vec2& top_right_corner) {
  const ci::Rectf kDioBaseCoords(styles::kBackgroundLength_ - styles::kBaseLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                 styles::kBackgroundLength_ + top_right_corner.x,
                                 styles::kBackgroundHeight_);

  for (auto& unit_pair : jojo_units_) {
    Unit& unit = unit_pair.first;

    if ((unit.CheckCollision(kDioBaseCoords, top_right_corner) ||
        unit.CheckInRange(kDioBaseCoords, top_right_corner)) &&
        unit.CanAttack() && !CheckEnemyCollision(unit, top_right_corner)) {
      dio_health_ -= unit.GetAttackPower();
      unit.StopTimer();
      unit.StopRangeAttackTimer();
      unit.PlayAttackSound();
    }
  }

  const ci::Rectf kJojoBaseCoords(top_right_corner.x,
                                  styles::kBackgroundHeight_ - styles::kBaseHeight_,
                                  styles::kBaseLength_ + top_right_corner.x,
                                  styles::kBackgroundHeight_);

  for (auto& unit_pair : dio_units_) {
    Unit& unit = unit_pair.first;

    if ((unit.CheckCollision(kJojoBaseCoords, top_right_corner) ||
        unit.CheckInRange(kJojoBaseCoords, top_right_corner)) &&
        unit.CanAttack() && !CheckEnemyCollision(unit, top_right_corner)) {
      jojo_health_ -= unit.GetAttackPower();
      unit.StopTimer();
      unit.StopRangeAttackTimer();
      unit.PlayAttackSound();
    }
  }
}

void GameState::UpdateUnitsHealth(const glm::vec2& top_right_corner) {

  for (auto& unit_pair : jojo_units_) {
    Unit& unit = unit_pair.first;

    if (unit.CanAttack() && !dio_units_.empty()) {
      dio_units_[0].first.UpdateHealth(unit.GetAttackPower());
      unit.StopTimer();
      unit.StopRangeAttackTimer();
      unit.PlayAttackSound();
    }
  }

  for (auto& unit_pair : dio_units_) {
    Unit& unit = unit_pair.first;

    if (unit.CanAttack() && !jojo_units_.empty()) {
      jojo_units_[0].first.UpdateHealth(unit.GetAttackPower());
      unit.StopTimer();
      unit.StopRangeAttackTimer();
      unit.PlayAttackSound();
    }
  }
}

void GameState::RemoveDeadUnits() {
  std::vector<std::pair<Unit, int>> alive_jojo_units;
  for (const auto& pair : jojo_units_) {
    if (pair.first.IsAlive()) {
      std::pair<Unit, int> alive_unit(pair.first, alive_jojo_units.size());
      alive_jojo_units.emplace_back(alive_unit);
    } else {
      jojo_experience_ += (int) ((float) pair.first.GetCost() * 0.5f);
      dio_experience_ += pair.first.GetCost();
      dio_money_ += (int) ((float) pair.first.GetCost() * 1.5f);
      pair.first.PlayDeathSound();
    }
  }

  std::vector<std::pair<Unit, int>> alive_dio_units;
  for (const auto& pair : dio_units_) {
    if (pair.first.IsAlive()) {
      std::pair<Unit, int> alive_unit(pair.first, alive_dio_units.size());
      alive_dio_units.emplace_back(alive_unit);
    } else {
      dio_experience_ += (int) ((float) pair.first.GetCost() * 0.5f);
      jojo_experience_ += (int) ((float) pair.first.GetCost() * 1.5f);
      jojo_money_ += pair.first.GetCost();
      pair.first.PlayDeathSound();
    }
  }

  jojo_units_ = alive_jojo_units;
  dio_units_ = alive_dio_units;
}

void GameState::UpdateCanUnitAttack(const glm::vec2 &top_right_corner,
                                    const ci::Rectf& jojo_base_coords, const ci::Rectf& dio_base_coords) {
  for (auto& pair : jojo_units_) {
    if (CheckEnemyCollision(pair.first, top_right_corner) ||
        pair.first.CheckCollision(dio_base_coords, top_right_corner)) {
      pair.first.ContinueTimer();
      pair.first.StopRangeAttackTimer();
    } else if (CheckEnemeyInRange(pair.first, top_right_corner, dio_base_coords)) { // Only Slingers have > 0 attack range
      pair.first.ContinueRangeAttackTimer();
    } else {
      pair.first.StopTimer();
    }
  }

  for (auto& pair : dio_units_) {
    if (CheckEnemyCollision(pair.first, top_right_corner) ||
        pair.first.CheckCollision(jojo_base_coords, top_right_corner)) {
      pair.first.ContinueTimer();
    } else if (CheckEnemeyInRange(pair.first, top_right_corner, jojo_base_coords)) { // Only Slingers have > 0 attack range
      pair.first.ContinueRangeAttackTimer();
    } else {
      pair.first.StopTimer();
    }
  }
}

bool GameState::CheckEnemeyInRange(const Unit &unit, const glm::vec2 &top_right_corner,
                                   const ci::Rectf& entity_hitbox) const {

  ci::Rectf enemy_hitbox;
  if (unit.IsTeamJojo() && !dio_units_.empty()) {
    enemy_hitbox = dio_units_[0].first.GetRectHitbox(top_right_corner);
  } else if (!unit.IsTeamJojo() && !jojo_units_.empty()) {
    enemy_hitbox = jojo_units_[0].first.GetRectHitbox(top_right_corner);
  }

  return unit.CheckInRange(enemy_hitbox, top_right_corner) || unit.CheckInRange(entity_hitbox, top_right_corner);
}

void GameState::UpgradeEra(bool is_team_jojo) {
  Era era;
  int experience;
  if (is_team_jojo) {
    era = jojo_era_;
    experience = jojo_experience_;
  } else {
    era = dio_era_;
    experience = dio_experience_;
  }

  auto era_numeric = static_cast<size_t>(era);
  if (era == kPart5 || experience < player_values::GetRequiredExperience().at(era)) {
    return;
  } else {
    era_numeric += 1;
  }

  if (is_team_jojo) {
    jojo_era_ = static_cast<Era>(era_numeric);
  } else {
    dio_era_ = static_cast<Era>(era_numeric);
  }

  dio_era_ = jojo_era_;
  UpgradeBaseHealth(true);
  UpgradeBaseHealth(false);
}

void GameState::UpgradeBaseHealth(bool is_team_jojo) {
  Era era;
  if (is_team_jojo) {
    era = jojo_era_;
  } else {
    era = dio_era_;
  }

  if (era == kPart1) {
    return;
  }

  auto previous_era = static_cast<Era>(static_cast<int>(era) - 1);

  std::unordered_map<Era, float> era_health = player_values::GetEraHealth();
  if (is_team_jojo) {
    jojo_health_ += era_health.at(era) - era_health.at(previous_era);
  } else {
    dio_health_ += era_health.at(era) - era_health.at(previous_era);
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
  Unit unit = unit_pair.first;
  size_t unit_index = unit_pair.second;

  if (unit.IsTeamJojo() && unit_index > 0) {
      ci::Rectf jojo_unit_hitbox = jojo_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      return unit.CheckCollision(jojo_unit_hitbox, top_right_corner);
  } else if (unit_index > 0) {
      ci::Rectf dio_unit_hitbox = dio_units_[unit_index - 1].first.GetRectHitbox(top_right_corner);
      return unit.CheckCollision(dio_unit_hitbox, top_right_corner);
  }

  return false;
}

bool GameState::CheckUnitCollisions(const std::pair<Unit, int>& unit_pair, const glm::vec2& top_right_corner) const {
  return CheckEnemyCollision(unit_pair.first, top_right_corner) ||
         CheckAlliedCollision(unit_pair, top_right_corner);
}

}