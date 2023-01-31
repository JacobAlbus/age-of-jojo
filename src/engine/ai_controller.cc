#include "engine/ai_controller.h"
#include <cstdlib>
#include <random>
#include "engine/consts.h"

namespace age_of_jojo {

AIController::AIController() : timer_(ci::Timer()),
                               kRequiredExperience_(player_values::GetRequiredExperience()) {
  timer_.start();

  unit_to_action_.insert(std::make_pair(Infantry, kSendInfantry));
  unit_to_action_.insert(std::make_pair(Slinger, kSendSlinger));
  unit_to_action_.insert(std::make_pair(Heavy, kSendHeavy));
  time_ = 0.0;
}

ControllerAction AIController::CalculateCurrentAction(const GameState& current_state, bool is_team_jojo) {
  time_ = timer_.getSeconds();

  Era era;
  int experience;
  int money;
  if (is_team_jojo) {
    era = current_state.GetJojoEra();
    experience = current_state.GetJojoExperience();
    money = current_state.GetJojoMoney();
  } else {
    era = current_state.GetDioEra();
    experience = current_state.GetDioExperience();
    money = current_state.GetDioMoney();
  }

  float reaction_time = 5.0f / styles::kGameSpeed_;

  if (timer_.getSeconds() < reaction_time || money == 0) {
    return kIdle;
  } else if (experience > kRequiredExperience_.at(era)) {
    return kUpgrade;
  } else if (!unit_queue_.empty()) {
    ControllerAction action = unit_to_action_.at(unit_queue_[0]);
    unit_queue_.erase(unit_queue_.begin());
    return action;
  }

  ResetTimer();
  std::unordered_map<UnitType, float> unit_prob_distribution = GetUnitProbDistribution(experience, era);

  int money_ratio = (int) money / unit_values::GetUnitCosts().at(era).at(Heavy);
  auto num_units_send = (size_t) GetNumUnitsSend(money_ratio);

  if (num_units_send == 0) {
    return kIdle;
  }

  for (size_t index = 0; index < num_units_send; index += 1) {
    unit_queue_.emplace_back(GetRandomUnit(unit_prob_distribution));
  }

  ControllerAction action = unit_to_action_.at(unit_queue_[0]);
  unit_queue_.erase(unit_queue_.begin());
  return action;
}

UnitType AIController::GetRandomUnit(const std::unordered_map<UnitType, float> &unit_prob_distribution) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, RAND_MAX);
  float random_number = (float) distr(gen) / RAND_MAX;

  if (random_number < unit_prob_distribution.at(Infantry)) {
    return Infantry;
  } else if (random_number < unit_prob_distribution.at(Slinger)) {
    return Slinger;
  } else {
    return Heavy;
  }
}

std::unordered_map<UnitType, float> AIController::GetUnitProbDistribution(int experience,
                                                                          Era era) const {
  if (IsEraDeveloped(experience, era)) {
    return std::unordered_map<UnitType, float>{{Infantry, 0.5f}, {Slinger, 0.8f}, {Heavy, 1.0f}};
  } else {
    return std::unordered_map<UnitType, float>{{Infantry, 0.6f}, {Slinger, 1.0f}, {Heavy, 0.0f}};
  }
}

int AIController::GetNumUnitsSend(int money_ratio) const {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> distr(1, 3);
//  return distr(gen);
  if (money_ratio == 0) {
    std::uniform_int_distribution<> distr(0, 1);
    return distr(gen);
  } else if (money_ratio == 1) {
    std::uniform_int_distribution<> distr(1, 1);
    return distr(gen);
  } else if (money_ratio == 2) {
    std::uniform_int_distribution<> distr(1, 2);
    return distr(gen);
  } else if (money_ratio == 3) {
    std::uniform_int_distribution<> distr(1, 2);
    return distr(gen);
  } else if (money_ratio == 4) {
    std::uniform_int_distribution<> distr(2, 2);
    return distr(gen);
  } else {
    std::uniform_int_distribution<> distr(3, 4);
    return distr(gen);
  }
}

bool AIController::IsEraDeveloped(int experience, Era era) const {
  if (era == kPart1) {
    return experience > (player_values::GetRequiredExperience().at(kPart1) / 2);
  }

  auto era_numeric = static_cast<size_t>(era);
  Era prev_era = static_cast<Era>(era_numeric - 1);
  int prev_era_experience = player_values::GetRequiredExperience().at(prev_era);
  int required_experience = player_values::GetRequiredExperience().at(era);

  return experience > ((prev_era_experience + required_experience) / 2);
}

void AIController::ResetTimer() {
  timer_.stop();
  timer_.start();
}
void AIController::ResetController() {
  ResetTimer();
  unit_queue_ = std::vector<UnitType>();
}

}
