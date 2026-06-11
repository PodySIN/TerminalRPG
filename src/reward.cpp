#include "reward.hpp"
#include <iostream>
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
  std::cout << "❤️ HP increased by " << amount_ << "\n";
}

rpg::ResourceReward::ResourceReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Resource"),
  amount_(amount)
{}

void rpg::ResourceReward::apply(Hero& hero) const
{
  hero.getStats().getResource().addFlat(amount_);
  hero.getStats().getCurrentResource() += amount_;
  std::cout << "💙 Resource increased by " << amount_ << "\n";
}

rpg::DamageReward::DamageReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Damage"),
  amount_(amount)
{}

void rpg::DamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addFlat(amount_);
  std::cout << "⚔️ Damage increased by " << amount_ << "\n";
}

rpg::DefenseReward::DefenseReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Defense"),
  amount_(amount)
{}

void rpg::DefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addFlat(amount_);
  std::cout << "🛡️ Defense increased by " << amount_ << "\n";
}

rpg::SpeedReward::SpeedReward(float amount):
  Reward("+" + std::to_string((int)amount) + " Speed"),
  amount_(amount)
{}

void rpg::SpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addBase(amount_);
  std::cout << "⚡ Speed increased by " << amount_ << "\n";
}

rpg::CritChanceReward::CritChanceReward(float amount):
  Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Chance"),
  amount_(amount)
{}

void rpg::CritChanceReward::apply(Hero& hero) const
{
  hero.getStats().getCritChance().addBase(amount_);
  std::cout << "🎯 Crit chance increased by " << amount_ * 100 << "%\n";
}

rpg::CritDamageReward::CritDamageReward(float amount):
  Reward("+" + std::to_string((int)(amount * 100)) + "% Crit Damage"),
  amount_(amount)
{}

void rpg::CritDamageReward::apply(Hero& hero) const
{
  hero.getStats().getCritDamage().addBase(amount_);
  std::cout << "💥 Crit damage increased by " << amount_ * 100 << "%\n";
}

rpg::SkillLevelReward::SkillLevelReward(size_t skill_index, size_t levels):
  Reward("Upgrade skill +" + std::to_string(levels) + " levels"),
  skill_index_(skill_index),
  levels_(levels)
{}

void rpg::SkillLevelReward::apply(Hero& hero) const
{
  if (skill_index_ < hero.getSkillManager().getSkillCount() && !hero.getSkillManager().isSkillLocked(skill_index_)) {
    hero.getSkillManager().addLevelsToSkill(skill_index_, levels_);
    std::cout << "📈 Skill upgraded!\n";
  } else {
    std::cout << "⚠️ Skill locked or not found!\n";
  }
}

rpg::PercentHealthReward::PercentHealthReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Max HP"),
  percent_(percent)
{}

void rpg::PercentHealthReward::apply(Hero& hero) const
{
  hero.getStats().getHealth().addMultiply(percent_ / 100.0f);
  std::cout << "❤️ Max HP increased by " << percent_ << "%\n";
}

rpg::PercentDamageReward::PercentDamageReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Damage"),
  percent_(percent)
{}

void rpg::PercentDamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addMultiply(percent_ / 100.0f);
  std::cout << "⚔️ Damage increased by " << percent_ << "%\n";
}

rpg::PercentDefenseReward::PercentDefenseReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Defense"),
  percent_(percent)
{}

void rpg::PercentDefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addMultiply(percent_ / 100.0f);
  std::cout << "🛡️ Defense increased by " << percent_ << "%\n";
}

rpg::PercentSpeedReward::PercentSpeedReward(float percent):
  Reward("+" + std::to_string((int)percent) + "% Speed"),
  percent_(percent)
{}

void rpg::PercentSpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addMultiply(percent_ / 100.0f);
  std::cout << "⚡ Speed increased by " << percent_ << "%\n";
}

bool rpg::UnlockSkillReward::hasLockedSkills(const Hero& hero)
{
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (hero.getSkillManager().isSkillLocked(i)) {
      return true;
    }
  }
  return false;
}

rpg::UnlockSkillReward::UnlockSkillReward():
  Reward("Unlock new skill")
{}

void rpg::UnlockSkillReward::apply(Hero& hero) const
{
  std::vector< size_t > locked_indices;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (hero.getSkillManager().isSkillLocked(i)) {
      locked_indices.push_back(i);
    }
  }

  if (locked_indices.empty()) {
    std::cout << "⚠️ No locked skills available! You get a skill level "
                 "upgrade instead.\n";
    hero.getSkillManager().addLevelsToSkill(0, 1);
    return;
  }

  int random_index = Random::getInt(0, locked_indices.size() - 1);
  size_t skill_to_unlock = locked_indices[random_index];

  hero.getSkillManager().unlockSkill(skill_to_unlock);
  std::cout << "🔓 New skill unlocked: " << hero.getSkillManager().getSkillName(skill_to_unlock) << "\n";
}

rpg::MultiSkillLevelReward::MultiSkillLevelReward(size_t levels):
  Reward("Upgrades skill on +" + std::to_string(levels) + " levels"),
  levels_(levels)
{}

void rpg::MultiSkillLevelReward::apply(Hero& hero) const
{
  std::vector< size_t > unlocked_indices;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (!hero.getSkillManager().isSkillLocked(i)) {
      unlocked_indices.push_back(i);
    }
  }

  if (unlocked_indices.empty()) {
    std::cout << "Dont have available skills!\n";
    return;
  }

  int random_index = Random::getInt(0, unlocked_indices.size() - 1);
  size_t skill_id = unlocked_indices[random_index];

  hero.getSkillManager().addLevelsToSkill(skill_id, levels_);
  std::cout << "📈 Random skill +" << levels_ << " level!\n";
}

rpg::FullHealReward::FullHealReward():
  Reward("Fully restore HP and Resource")
{}

void rpg::FullHealReward::apply(Hero& hero) const
{
  hero.getStats().getCurrentHealth() = hero.getStats().getHealth().getTotal();
  hero.getStats().getCurrentResource() = hero.getStats().getResource().getTotal();
  std::cout << "💚 Fully healed!\n";
}

std::unique_ptr< rpg::Reward > rpg::RewardFactory::createRandomCommonReward(int floor)
{
  int type = Random::getInt(0, 8);
  float bonus = 1.0f + (floor - 1) * 0.2f;

  switch (type) {
    case 0:
      return std::make_unique< HealthReward >(30.0f * bonus);
    case 1:
      return std::make_unique< ResourceReward >(20.0f * bonus);
    case 2:
      return std::make_unique< DamageReward >(10.0f * bonus);
    case 3:
      return std::make_unique< DefenseReward >(10.0f * bonus);
    case 4:
      return std::make_unique< SpeedReward >(10.0f * bonus);
    case 5:
      return std::make_unique< CritChanceReward >(0.02f);
    case 6:
      return std::make_unique< CritDamageReward >(0.05f);
    case 7:
      return std::make_unique< SkillLevelReward >(0, 1);
    case 8:
      return std::make_unique< SkillLevelReward >(1, 1);
    default:
      return std::make_unique< HealthReward >(30.0f * bonus);
  }
}

std::unique_ptr< rpg::Reward > rpg::RewardFactory::createRandomRareReward(const Hero& hero, int)
{
  std::vector< std::unique_ptr< Reward > > available;

  available.push_back(std::make_unique< PercentHealthReward >(20.0f));
  available.push_back(std::make_unique< PercentDamageReward >(15.0f));
  available.push_back(std::make_unique< PercentDefenseReward >(20.0f));
  available.push_back(std::make_unique< PercentSpeedReward >(15.0f));

  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (hero.getSkillManager().isSkillLocked(i)) {
      available.push_back(std::make_unique< UnlockSkillReward >());
      break;
    }
  }

  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (!hero.getSkillManager().isSkillLocked(i)) {
      available.push_back(std::make_unique< MultiSkillLevelReward >(4));
      break;
    }
  }

  if (hero.getStats().getCurrentHealth() < hero.getStats().getHealth().getTotal()) {
    available.push_back(std::make_unique< FullHealReward >());
  }

  int random_index = Random::getInt(0, available.size() - 1);
  return std::move(available[random_index]);
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateCommonRewards(int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;
  rewards.push_back(createRandomCommonReward(floor));
  rewards.push_back(createRandomCommonReward(floor));
  rewards.push_back(createRandomCommonReward(floor));
  return rewards;
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::RewardFactory::generateRareRewards(const Hero& hero, int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;

  for (int i = 0; i < 3; i++) {
    rewards.push_back(createRandomRareReward(hero, floor));
  }

  return rewards;
}
