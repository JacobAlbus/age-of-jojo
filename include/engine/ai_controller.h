#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_

#include "engine/base_controller.h"

namespace age_of_jojo {

class AIController : public BaseController {
 public:

  AIController();

  ControllerAction CalculateCurrentAction(const GameState& current_state, bool is_team_jojo) override;

  inline double GetTimerSeconds() const override { return timer_.getSeconds(); };

  void ResetController() override;

 private:

  bool IsEraDeveloped(int experience, Era era) const;

  std::unordered_map<UnitType, float> GetUnitProbDistribution(int experience, Era era) const;

  UnitType GetRandomUnit(const std::unordered_map<UnitType, float>& unit_prob_distribution) const;

  int GetNumUnitsSend(int money_ratio) const;

  void ResetTimer();

  double time_;

  ci::Timer timer_;
  std::vector<UnitType> unit_queue_;
  std::unordered_map<UnitType, ControllerAction> unit_to_action_;
  const std::unordered_map<Era, int> kRequiredExperience_;
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_AI_CONTROLLER_H_
