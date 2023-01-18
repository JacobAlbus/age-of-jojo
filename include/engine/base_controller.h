#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_

#include "engine/game_state.h"
#include "engine/consts.h"

namespace age_of_jojo {

enum ControllerAction {
  kIdle,
  kSendInfantry,
  kSendSlinger,
  kSendHeavy,
  kBuildTurret,
  kBuyTurretSmall,
  kBuyTurretMedium,
  kBuyTurretBig,
  kUpgrade
};

class BaseController {
 public:

  inline BaseController() {
    unit_costs_.insert(std::make_pair(kPart1, std::unordered_map<UnitType, size_t>()));
    unit_costs_.insert(std::make_pair(kPart2, std::unordered_map<UnitType, size_t>()));
    unit_costs_.insert(std::make_pair(kPart3, std::unordered_map<UnitType, size_t>()));
    unit_costs_.insert(std::make_pair(kPart4, std::unordered_map<UnitType, size_t>()));
    unit_costs_.insert(std::make_pair(kPart5, std::unordered_map<UnitType, size_t>()));

    unit_costs_.at(kPart1).insert(std::make_pair(Infantry, unit_values::kPart1InfantryCost_));
    unit_costs_.at(kPart1).insert(std::make_pair(Infantry, unit_values::kPart1SlingerCost_));
    unit_costs_.at(kPart1).insert(std::make_pair(Infantry, unit_values::kPart1HeavyCost_));

    unit_costs_.at(kPart2).insert(std::make_pair(Infantry, unit_values::kPart2InfantryCost_));
    unit_costs_.at(kPart2).insert(std::make_pair(Infantry, unit_values::kPart2SlingerCost_));
    unit_costs_.at(kPart2).insert(std::make_pair(Infantry, unit_values::kPart2HeavyCost_));

    unit_costs_.at(kPart3).insert(std::make_pair(Infantry, unit_values::kPart3InfantryCost_));
    unit_costs_.at(kPart3).insert(std::make_pair(Infantry, unit_values::kPart3SlingerCost_));
    unit_costs_.at(kPart3).insert(std::make_pair(Infantry, unit_values::kPart3HeavyCost_));

    unit_costs_.at(kPart4).insert(std::make_pair(Infantry, unit_values::kPart4InfantryCost_));
    unit_costs_.at(kPart4).insert(std::make_pair(Infantry, unit_values::kPart4SlingerCost_));
    unit_costs_.at(kPart4).insert(std::make_pair(Infantry, unit_values::kPart4HeavyCost_));

    unit_costs_.at(kPart5).insert(std::make_pair(Infantry, unit_values::kPart5InfantryCost_));
    unit_costs_.at(kPart5).insert(std::make_pair(Infantry, unit_values::kPart5SlingerCost_));
    unit_costs_.at(kPart5).insert(std::make_pair(Infantry, unit_values::kPart5HeavyCost_));
  }

  inline virtual ~BaseController() = default;

  virtual ControllerAction CalculateCurrentAction(const GameState& current_state) = 0;

  virtual double GetTimerSeconds() const = 0;

 private:

  std::unordered_map<Era, std::unordered_map<UnitType, size_t>> unit_costs_;

  inline bool CanAfford(Era unit_era, UnitType unit_type, size_t player_money) {
    return unit_costs_.at(unit_era).at(unit_type) <= player_money;
  }
};

}

#endif //AGE_OF_JOJO_INCLUDE_ENGINE_BASE_CONTROLLER_H_
