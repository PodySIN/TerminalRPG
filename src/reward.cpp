#include "reward.hpp"
#include "random.hpp"
#include <iostream>
#include <memory>

namespace rpg {

  Reward::Reward(const std::string& description) : description_(description)
  {
  }

  std::string Reward::getDescription() const
  {
    return description_;
  }

  HealthReward::HealthReward(float amount) :
    Reward("+" + std::to_string((int)amount) + " HP"), amount_(amount)
  {
  }

  void HealthReward::apply(Hero& hero) const
  {
    hero.getStats().getHealth().addFlat(amount_);
    hero.getStats().getCurrentHealth() += amount_;
    std::cout << "HP increased by " << amount_ << "\n";
  }

  ResourceReward::ResourceReward(float amount) :
    Reward("+" + std::to_string((int)amount) + " Resource"), amount_(amount)
  {
  }

  void ResourceReward::apply(Hero& hero) const
  {
    hero.getStats().getResource().addFlat(amount_);
    hero.getStats().getCurrentResource() += amount_;
    std::cout << "Resource increased by " << amount_ << "\n";
  }

  DamageReward::DamageReward(float amount) :
    Reward("+" + std::to_string((int)amount) + " Damage"), amount_(amount)
  {
  }

  void DamageReward::apply(Hero& hero) const
  {
    hero.getStats().getDamage().addFlat(amount_);
    std::cout << "Damage increased by " << amount_ << "\n";
  }

  DefenseReward::DefenseReward(float amount) :
    Reward("+" + std::to_string((int)amount) + " Defense"), amount_(amount)
  {
  }

  void DefenseReward::apply(Hero& hero) const
  {
    hero.getStats().getDefense().addFlat(amount_);
    std::cout << "Defense increased by " << amount_ << "\n";
  }

  SpeedReward::SpeedReward(float amount) :
    Reward("+" + std::to_string((int)amount) + " Speed"), amount_(amount)
  {
  }

  void SpeedReward::apply(Hero& hero) const
  {
    hero.getStats().getSpeed().addBase(amount_);
    std::cout << "Speed increased by " << amount_ << "\n";
  }

  CritChanceReward::CritChanceReward(float amount) :
    Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Chance"),
    amount_(amount)
  {
  }

  void CritChanceReward::apply(Hero& hero) const
  {
    hero.getStats().getCritChance().addBase(amount_);
    std::cout << "Crit chance increased by " << amount_ * 100 << "%\n";
  }

  CritDamageReward::CritDamageReward(float amount) :
    Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Damage"),
    amount_(amount)
  {
  }

  void CritDamageReward::apply(Hero& hero) const
  {
    hero.getStats().getCritDamage().addBase(amount_);
    std::cout << "Crit damage increased by " << amount_ * 100 << "%\n";
  }

  PercentHealthReward::PercentHealthReward(float percent) :
    Reward("+" + std::to_string((int)percent) + "% Max HP"), percent_(percent)
  {
  }

  void PercentHealthReward::apply(Hero& hero) const
  {
    hero.getStats().getHealth().addMultiply(percent_ / 100.0f);
    std::cout << "Max HP increased by " << percent_ << "%\n";
  }

  PercentDamageReward::PercentDamageReward(float percent) :
    Reward("+" + std::to_string((int)percent) + "% Damage"), percent_(percent)
  {
  }

  void PercentDamageReward::apply(Hero& hero) const
  {
    hero.getStats().getDamage().addMultiply(percent_ / 100.0f);
    std::cout << "Damage increased by " << percent_ << "%\n";
  }

  std::unique_ptr< Reward > RewardFactory::createRandomCommonReward()
  {
    int type = Random::getInt(0, 8);

    switch (type) {
      case 0:
        return std::make_unique< HealthReward >(30.0f);
      case 1:
        return std::make_unique< ResourceReward >(20.0f);
      case 2:
        return std::make_unique< DamageReward >(10.0f);
      case 3:
        return std::make_unique< DefenseReward >(10.0f);
      case 4:
        return std::make_unique< SpeedReward >(10.0f);
      case 6:
        return std::make_unique< CritChanceReward >(0.02f);
      case 7:
        return std::make_unique< CritDamageReward >(0.04f);
      default:
        return std::make_unique< HealthReward >(30.0f);
    }
  }

  std::unique_ptr< Reward > RewardFactory::createRandomRareReward()
  {
    int type = Random::getInt(0, 2);

    switch (type) {
      case 0:
        return std::make_unique< PercentHealthReward >(20.0f);
      case 1:
        return std::make_unique< PercentDamageReward >(15.0f);
      default:
        return std::make_unique< PercentHealthReward >(20.0f);
    }
  }

} // namespace rpg
