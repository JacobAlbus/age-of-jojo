#pragma once
#ifndef AGE_OF_JOJO_INCLUDE_ENGINE_CONSTS_H_
#define AGE_OF_JOJO_INCLUDE_ENGINE_CONSTS_H_

enum Era {
  kPart1,
  kPart2,
  kPart3,
  kPart4,
  kPart5
};

enum GameMode {
  kPlaying_,
  kJojoWins_,
  kDioWins_,
};

enum UnitType {
  Infantry,
  Slinger,
  Heavy
};

namespace styles {
  const float kBaseLength_ = 264;
  const float kBaseHeight_ = 415;
  const float kBackgroundLength_ = 2000;
  const float kBackgroundHeight_ = 900;
  const float kWindowSize_ = 900;
}

namespace unit_values {
  const float kInfantryWidth_ = 50;
  const float kInfantryHeight_ = 100;
  const float kHeavyWidth_ = 80;
  const float kHeavyHeight_ = 140;

  const float kPart1InfantryHealth_ = 100;
  const int kPart1InfantryCost_ = 15;
  const int kPart1SlingerCost_ = 25;
  const float kPart1HeavyHealth_ = 250;
  const int kPart1HeavyCost_ = 100;

  const int kPart2InfantryCost_ = 50;
  const int kPart2SlingerCost_ = 75;
  const int kPart2HeavyCost_ = 500;

  const int kPart3InfantryCost_ = 200;
  const int kPart3SlingerCost_ = 400;
  const int kPart3HeavyCost_ = 1000;

  const int kPart4InfantryCost_ = 1500;
  const int kPart4SlingerCost_ = 2000;
  const int kPart4HeavyCost_ = 7000;

  const int kPart5InfantryCost_ = 5000;
  const int kPart5SlingerCost_ = 6000;
  const int kPart5HeavyCost_ = 20000;
}

namespace player_values {
  const float kAge1Health_ = 500;
  const float kAge2Health_ = 1100;
  const float kAge3Health_ = 2000;
  const float kAge4Health_ = 3200;
  const float kAge5Health_ = 4700;

  const int kAge2Experience_ = 4000;
  const int kAge3Experience_ = 14000;
  const int kAge4Experience_ = 45000;
  const int kAge5Experience_ = 200000;

static std::unordered_map<Era, float> GetEraHealth() {
  std::unordered_map<Era, float> health;

  health.insert(std::make_pair(kPart1, 500.0f));
  health.insert(std::make_pair(kPart2, 1100.0f));
  health.insert(std::make_pair(kPart3, 2000.0f));
  health.insert(std::make_pair(kPart4, 3200.0f));
  health.insert(std::make_pair(kPart5, 4700.0f));

  return health;
}

static std::unordered_map<Era, int> GetRequiredExperience() {
  std::unordered_map<Era, int> required_experience;

//  required_experience.insert(std::make_pair(kPart1, 4000));
//  required_experience.insert(std::make_pair(kPart2, 14000));
//  required_experience.insert(std::make_pair(kPart3, 45000));
//  required_experience.insert(std::make_pair(kPart4, 200000));
  required_experience.insert(std::make_pair(kPart1, 100));
  required_experience.insert(std::make_pair(kPart2, 300));
  required_experience.insert(std::make_pair(kPart3, 1000));
  required_experience.insert(std::make_pair(kPart4, 3000));

  return required_experience;
}

  const int kStartingMoney_ = 175;
}

namespace game_values {
  const int kButtonSize_ = 60;
  const glm::vec2 kForbiddenMousePos_(-456, -2104);
  
  const ci::Rectf kInfantryButtonPos_(30, 30, 30 + kButtonSize_, 30 + kButtonSize_);
  const ci::Rectf kSlingerButtonPos_(100, 30, 100 + kButtonSize_, 30 + kButtonSize_);
  const ci::Rectf kHeavyButtonPos_(170, 30, 170 + kButtonSize_, 30 + kButtonSize_);
  
  const ci::Rectf kSmallTurretButtonPos_(30, 100, 30 + kButtonSize_, 100 + kButtonSize_);
  const ci::Rectf kMediumTurretButtonPos_(100, 100, 100 + kButtonSize_, 100 + kButtonSize_);
  const ci::Rectf kLargeTurretButtonPos_(170, 100, 170 + kButtonSize_, 100 + kButtonSize_);

  const ci::Rectf kSellTurretButtonPos_(30, 170, 30 + kButtonSize_, 170 + kButtonSize_);
  const ci::Rectf kAddTurretButtonPos_(100, 170, 100 + kButtonSize_, 170 + kButtonSize_);
  const ci::Rectf kUpgradeButtonPos_(170, 170, 170 + kButtonSize_, 170 + kButtonSize_);

  const int kMaxQueueLength_ = 5;
  const float kQueueSlotSize_ = 20;
  const ci::Rectf kQueueSlot1_(styles::kWindowSize_ - (2 * kQueueSlotSize_), 2 * kQueueSlotSize_,
                               styles::kWindowSize_ - kQueueSlotSize_, 3 * kQueueSlotSize_);
}

namespace unit_values {
static std::unordered_map<Era, std::unordered_map<UnitType, int>> GetUnitCosts() {
  std::unordered_map<Era, std::unordered_map<UnitType, int>> unit_costs;

  unit_costs.insert(std::make_pair(kPart1, std::unordered_map<UnitType, int>()));
  unit_costs.insert(std::make_pair(kPart2, std::unordered_map<UnitType, int>()));
  unit_costs.insert(std::make_pair(kPart3, std::unordered_map<UnitType, int>()));
  unit_costs.insert(std::make_pair(kPart4, std::unordered_map<UnitType, int>()));
  unit_costs.insert(std::make_pair(kPart5, std::unordered_map<UnitType, int>()));

  unit_costs.at(kPart1).insert(std::make_pair(Infantry, unit_values::kPart1InfantryCost_));
  unit_costs.at(kPart1).insert(std::make_pair(Slinger, unit_values::kPart1SlingerCost_));
  unit_costs.at(kPart1).insert(std::make_pair(Heavy, unit_values::kPart1HeavyCost_));

  unit_costs.at(kPart2).insert(std::make_pair(Infantry, unit_values::kPart2InfantryCost_));
  unit_costs.at(kPart2).insert(std::make_pair(Slinger, unit_values::kPart2SlingerCost_));
  unit_costs.at(kPart2).insert(std::make_pair(Heavy, unit_values::kPart2HeavyCost_));

  unit_costs.at(kPart3).insert(std::make_pair(Infantry, unit_values::kPart3InfantryCost_));
  unit_costs.at(kPart3).insert(std::make_pair(Slinger, unit_values::kPart3SlingerCost_));
  unit_costs.at(kPart3).insert(std::make_pair(Heavy, unit_values::kPart3HeavyCost_));

  unit_costs.at(kPart4).insert(std::make_pair(Infantry, unit_values::kPart4InfantryCost_));
  unit_costs.at(kPart4).insert(std::make_pair(Slinger, unit_values::kPart4SlingerCost_));
  unit_costs.at(kPart4).insert(std::make_pair(Heavy, unit_values::kPart4HeavyCost_));

  unit_costs.at(kPart5).insert(std::make_pair(Infantry, unit_values::kPart5InfantryCost_));
  unit_costs.at(kPart5).insert(std::make_pair(Slinger, unit_values::kPart5SlingerCost_));
  unit_costs.at(kPart5).insert(std::make_pair(Heavy, unit_values::kPart5HeavyCost_));

  return unit_costs;
}

static std::unordered_map<Era, std::unordered_map<UnitType, float>> GetUnitHealth() {
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_values;

  unit_values.insert(std::make_pair(kPart1, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart2, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart3, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart4, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart5, std::unordered_map<UnitType, float>()));

  unit_values.at(kPart1).insert(std::make_pair(Infantry, 20.0f));
  unit_values.at(kPart1).insert(std::make_pair(Slinger, 15.0f));
  unit_values.at(kPart1).insert(std::make_pair(Heavy, 100.0f));

  unit_values.at(kPart2).insert(std::make_pair(Infantry, 40.0f));
  unit_values.at(kPart2).insert(std::make_pair(Slinger, 30.0f));
  unit_values.at(kPart2).insert(std::make_pair(Heavy, 200.0f));

  unit_values.at(kPart3).insert(std::make_pair(Infantry, 80.0f));
  unit_values.at(kPart3).insert(std::make_pair(Slinger, 60.0f));
  unit_values.at(kPart3).insert(std::make_pair(Heavy, 400.0f));

  unit_values.at(kPart4).insert(std::make_pair(Infantry, 160.0f));
  unit_values.at(kPart4).insert(std::make_pair(Slinger, 120.0f));
  unit_values.at(kPart4).insert(std::make_pair(Heavy, 800.0f));

  unit_values.at(kPart5).insert(std::make_pair(Infantry, 320.0f));
  unit_values.at(kPart5).insert(std::make_pair(Slinger, 240.0f));
  unit_values.at(kPart5).insert(std::make_pair(Heavy, 1600.0f));

  return unit_values;
}

static std::unordered_map<Era, std::unordered_map<UnitType, float>> GetUnitAttackPower() {
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_values;

  unit_values.insert(std::make_pair(kPart1, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart2, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart3, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart4, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart5, std::unordered_map<UnitType, float>()));

  unit_values.at(kPart1).insert(std::make_pair(Infantry, 7.0f));
  unit_values.at(kPart1).insert(std::make_pair(Slinger, 4.0f));
  unit_values.at(kPart1).insert(std::make_pair(Heavy, 12.0f));

  unit_values.at(kPart2).insert(std::make_pair(Infantry, 14.0f));
  unit_values.at(kPart2).insert(std::make_pair(Slinger, 8.0f));
  unit_values.at(kPart2).insert(std::make_pair(Heavy, 24.0f));

  unit_values.at(kPart3).insert(std::make_pair(Infantry, 28.0f));
  unit_values.at(kPart3).insert(std::make_pair(Slinger, 16.0f));
  unit_values.at(kPart3).insert(std::make_pair(Heavy, 48.0f));

  unit_values.at(kPart4).insert(std::make_pair(Infantry, 56.0f));
  unit_values.at(kPart4).insert(std::make_pair(Slinger, 32.0f));
  unit_values.at(kPart4).insert(std::make_pair(Heavy, 96.0f));

  unit_values.at(kPart5).insert(std::make_pair(Infantry, 112.0f));
  unit_values.at(kPart5).insert(std::make_pair(Slinger, 64.0f));
  unit_values.at(kPart5).insert(std::make_pair(Heavy, 192.0f));

  return unit_values;
}

static std::unordered_map<Era, std::unordered_map<UnitType, float>> GetUnitDeploymentTime() {
  std::unordered_map<Era, std::unordered_map<UnitType, float>> unit_values;

  unit_values.insert(std::make_pair(kPart1, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart2, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart3, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart4, std::unordered_map<UnitType, float>()));
  unit_values.insert(std::make_pair(kPart5, std::unordered_map<UnitType, float>()));

  unit_values.at(kPart1).insert(std::make_pair(Infantry, 1.0f));
  unit_values.at(kPart1).insert(std::make_pair(Slinger, 1.0f));
  unit_values.at(kPart1).insert(std::make_pair(Heavy, 2.5f));

  unit_values.at(kPart2).insert(std::make_pair(Infantry, 1.3f));
  unit_values.at(kPart2).insert(std::make_pair(Slinger, 1.3f));
  unit_values.at(kPart2).insert(std::make_pair(Heavy, 3.0f));

  unit_values.at(kPart3).insert(std::make_pair(Infantry, 2.3f));
  unit_values.at(kPart3).insert(std::make_pair(Slinger, 2.3f));
  unit_values.at(kPart3).insert(std::make_pair(Heavy, 5.0f));

  unit_values.at(kPart4).insert(std::make_pair(Infantry, 2.0f));
  unit_values.at(kPart4).insert(std::make_pair(Slinger, 2.0f));
  unit_values.at(kPart4).insert(std::make_pair(Heavy, 6.5f));

  unit_values.at(kPart5).insert(std::make_pair(Infantry, 2.5f));
  unit_values.at(kPart5).insert(std::make_pair(Slinger, 2.5f));
  unit_values.at(kPart5).insert(std::make_pair(Heavy, 7.0f));

  return unit_values;
}
}


#endif //AGE_OF_JOJO_INCLUDE_ENGINE_CONSTS_H_
