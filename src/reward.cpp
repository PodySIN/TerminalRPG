#include "reward.hpp"
#include <memory>
#include <string>
#include "random.hpp"

rpg::Reward::Reward(const std::string& description):
  description_(description)
{}

std::string rpg::Reward::getDescription() const
{
  return description_;
}

rpg::HealthReward::HealthReward(float amount):
  Reward("+" + std::to_string((int)amount) + " HP"),
  amount_(amount)
{}

void rpg::HealthReward::apply(Hero& hero) const
{
  hero.getStats().getHealth().addFlat(amount_);
  hero.getStats().getCurrentHealth() += amount_;
}

rpg::ResourceReward::ResourceReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Resource"),
  amount_(amount)
{}

void rpg::ResourceReward::apply(Hero& hero) const
{
  hero.getStats().getResource().addFlat(amount_);
  hero.getStats().getCurrentResource() += amount_;
}

rpg::DamageReward::DamageReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Damage"),
  amount_(amount)
{}

void rpg::DamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addFlat(amount_);
}

rpg::DefenseReward::DefenseReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Defense"),
  amount_(amount)
{}

void rpg::DefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addFlat(amount_);
}

rpg::SpeedReward::SpeedReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Speed"),
  amount_(amount)
{}

void rpg::SpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addBase(amount_);
}

rpg::CritChanceReward::CritChanceReward(float amount):
  Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Chance"),
  amount_(amount)
{}

void rpg::CritChanceReward::apply(Hero& hero) const
{
  hero.getStats().getCritChance().addBase(amount_);
}

rpg::CritDamageReward::CritDamageReward(float amount):
  Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Damage"),
  amount_(amount)
{}

void rpg::CritDamageReward::apply(Hero& hero) const
{
  hero.getStats().getCritDamage().addBase(amount_);
}

rpg::SkillLevelReward::SkillLevelReward(size_t skill_index, size_t levels):
  Reward("Upgrade " + std::to_string(levels) + " lvl"),
  skill_index_(skill_index),
  levels_(levels)
{}

void rpg::SkillLevelReward::apply(Hero& hero) const
{
  if (skill_index_ < hero.getSkillManager().getSkillCount() && !hero.getSkillManager().isSkillLocked(skill_index_))
    hero.getSkillManager().addLevelsToSkill(skill_index_, levels_);
}

rpg::PercentHealthReward::PercentHealthReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Max HP"),
  percent_(percent)
{}

void rpg::PercentHealthReward::apply(Hero& hero) const
{
  float old_max = hero.getStats().getHealth().getTotal();
  hero.getStats().getHealth().addMultiply(percent_ / 100.0f);
  float new_max = hero.getStats().getHealth().getTotal();
  float increase = new_max - old_max;
  hero.getStats().getCurrentHealth() += increase;
  if (hero.getStats().getCurrentHealth() > new_max)
    hero.getStats().getCurrentHealth() = new_max;
}

rpg::PercentDamageReward::PercentDamageReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Damage"),
  percent_(percent)
{}

void rpg::PercentDamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addMultiply(percent_ / 100.0f);
}

rpg::PercentDefenseReward::PercentDefenseReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Defense"),
  percent_(percent)
{}

void rpg::PercentDefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addMultiply(percent_ / 100.0f);
}

rpg::PercentSpeedReward::PercentSpeedReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Speed"),
  percent_(percent)
{}

void rpg::PercentSpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addMultiply(percent_ / 100.0f);
}

rpg::UnlockSkillReward::UnlockSkillReward(bool include_ults):
  Reward("Unlock new skill"),
  include_ults_(include_ults)
{}

void rpg::UnlockSkillReward::apply(Hero& hero) const
{
  std::vector< size_t > locked;
  size_t count = hero.getSkillManager().getSkillCount();
  size_t limit = include_ults_ ? count : (count > 2 ? count - 2 : count);

  for (size_t i = 0; i < limit; i++) {
    if (hero.getSkillManager().isSkillLocked(i))
      locked.push_back(i);
  }

  if (locked.empty()) {
    hero.getSkillManager().addLevelsToSkill(0, 1);
    return;
  }

  hero.getSkillManager().unlockSkill(locked[Random::getInt(0, locked.size() - 1)]);
}

rpg::MultiSkillLevelReward::MultiSkillLevelReward(size_t levels):
  Reward("+" + std::to_string(levels) + " lvl to random skill"),
  levels_(levels)
{}

void rpg::MultiSkillLevelReward::apply(Hero& hero) const
{
  std::vector< size_t > unlocked;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++)
    if (!hero.getSkillManager().isSkillLocked(i))
      unlocked.push_back(i);
  if (unlocked.empty())
    return;
  hero.getSkillManager().addLevelsToSkill(unlocked[Random::getInt(0, unlocked.size() - 1)], levels_);
}

rpg::FullHealReward::FullHealReward(float pct):
  Reward("Heal " + std::to_string((int)(pct * 100)) + "% HP"),
  percent_(pct)
{}

void rpg::FullHealReward::apply(Hero& hero) const
{
  hero.getDamageManager().heal(hero.getStats().getHealth().getTotal() * percent_);
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateCommonRewards(int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;
  float b = 1.0f + (floor - 1) * 0.15f;
  rewards.push_back(std::make_unique< HealthReward >(20.0f * b));
  rewards.push_back(std::make_unique< DamageReward >(8.0f * b));
  rewards.push_back(std::make_unique< DefenseReward >(6.0f * b));
  rewards.push_back(std::make_unique< SpeedReward >(5.0f * b));
  return rewards;
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateRareRewards(const Hero&, int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;
  float b = 1.0f + (floor - 1) * 0.2f;
  rewards.push_back(std::make_unique< HealthReward >(40.0f * b));
  rewards.push_back(std::make_unique< DamageReward >(15.0f * b));
  rewards.push_back(std::make_unique< PercentHealthReward >(10.0f));
  rewards.push_back(std::make_unique< PercentDamageReward >(8.0f));
  rewards.push_back(std::make_unique< FullHealReward >(0.25f));
  return rewards;
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateEpicRewards(const Hero& hero, int floor)
{
  (void)floor;
  std::vector< std::unique_ptr< Reward > > rewards;

  bool found_locked = false;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (i >= hero.getSkillManager().getSkillCount() - 2)
      continue;
    if (hero.getSkillManager().isSkillLocked(i)) {
      found_locked = true;
      break;
    }
  }

  if (found_locked)
    rewards.push_back(std::make_unique< UnlockSkillReward >(false));
  else
    rewards.push_back(std::make_unique< MultiSkillLevelReward >(3));

  rewards.push_back(std::make_unique< PercentHealthReward >(20.0f));
  rewards.push_back(std::make_unique< PercentDamageReward >(15.0f));
  rewards.push_back(std::make_unique< FullHealReward >(0.5f));
  rewards.push_back(std::make_unique< CritChanceReward >(0.05f));
  rewards.push_back(std::make_unique< MultiSkillLevelReward >(2));
  return rewards;
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateLegendaryRewards(const Hero& hero, int floor)
{
  (void)floor;
  std::vector< std::unique_ptr< Reward > > rewards;

  bool found_locked = false;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (hero.getSkillManager().isSkillLocked(i)) {
      found_locked = true;
      break;
    }
  }

  if (found_locked)
    rewards.push_back(std::make_unique< UnlockSkillReward >(true));

  size_t ult = hero.getSkillManager().getSkillCount() - 1;
  if (!hero.getSkillManager().isSkillLocked(ult))
    rewards.push_back(std::make_unique< SkillLevelReward >(ult, 3));

  rewards.push_back(std::make_unique< PercentHealthReward >(35.0f));
  rewards.push_back(std::make_unique< PercentDamageReward >(25.0f));
  rewards.push_back(std::make_unique< FullHealReward >(1.0f));
  rewards.push_back(std::make_unique< CritDamageReward >(0.3f));
  rewards.push_back(std::make_unique< PercentDefenseReward >(20.0f));
  rewards.push_back(std::make_unique< PercentSpeedReward >(15.0f));
  return rewards;
}
