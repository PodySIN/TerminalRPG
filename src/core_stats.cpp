#include "core_stats.hpp"

float rpg::ComplexStat::getTotal() const
{
  return ((base_ + flat_) * multiply_) * amplify_;
}

float rpg::BaseStat::getBase() const
{
  return base_;
}

void rpg::BaseStat::addBase(float value)
{
  base_ += value;
}

void rpg::BaseStat::setBase(float value)
{
  base_ = value;
}

void rpg::ComplexStat::addFlat(float value)
{
  flat_ += value;
}

void rpg::ComplexStat::addAmplify(float value)
{
  amplify_ += value;
}

void rpg::ComplexStat::addMultiply(float value)
{
  multiply_ += value;
}

rpg::ComplexStat& rpg::CoreStats::getEvasion()
{
  return evasion_;
}

const rpg::ComplexStat& rpg::CoreStats::getEvasion() const
{
  return evasion_;
}

rpg::ComplexStat& rpg::CoreStats::getHealth()
{
  return health_;
}

const rpg::ComplexStat& rpg::CoreStats::getHealth() const
{
  return health_;
}

rpg::ComplexStat& rpg::CoreStats::getDefense()
{
  return defense_;
}

const rpg::ComplexStat& rpg::CoreStats::getDefense() const
{
  return defense_;
}

rpg::ComplexStat& rpg::CoreStats::getDamage()
{
  return damage_;
}

const rpg::ComplexStat& rpg::CoreStats::getDamage() const
{
  return damage_;
}

rpg::ComplexStat& rpg::CoreStats::getSpeed()
{
  return speed_;
}

const rpg::ComplexStat& rpg::CoreStats::getSpeed() const
{
  return speed_;
}

rpg::ComplexStat& rpg::CoreStats::getResource()
{
  return resource_;
}

const rpg::ComplexStat& rpg::CoreStats::getResource() const
{
  return resource_;
}

rpg::BaseStat& rpg::CoreStats::getEffectResistance()
{
  return effect_resistance_;
}

const rpg::BaseStat& rpg::CoreStats::getEffectResistance() const
{
  return effect_resistance_;
}

rpg::BaseStat& rpg::CoreStats::getDamageBonus()
{
  return damage_bonus_;
}

const rpg::BaseStat& rpg::CoreStats::getDamageBonus() const
{
  return damage_bonus_;
}

rpg::BaseStat& rpg::CoreStats::getCritDamage()
{
  return crit_damage_;
}

const rpg::BaseStat& rpg::CoreStats::getCritDamage() const
{
  return crit_damage_;
}

rpg::BaseStat& rpg::CoreStats::getCritChance()
{
  return crit_chance_;
}

const rpg::BaseStat& rpg::CoreStats::getCritChance() const
{
  return crit_chance_;
}

rpg::BaseStat& rpg::CoreStats::getDamageReduction()
{
  return damage_reduction_;
}

const rpg::BaseStat& rpg::CoreStats::getDamageReduction() const
{
  return damage_reduction_;
}

rpg::BaseStat& rpg::CoreStats::getBlockDamage()
{
  return block_damage_;
}

const rpg::BaseStat& rpg::CoreStats::getBlockDamage() const
{
  return block_damage_;
}

rpg::BaseStat& rpg::CoreStats::getMagicResistance()
{
  return magic_resistance_;
}

const rpg::BaseStat& rpg::CoreStats::getMagicResistance() const
{
  return magic_resistance_;
}

float rpg::CoreStats::calculatePhysicalResistance() const
{
  float defense = defense_.getTotal();
  return defense / (defense + 100.0f);
}

float& rpg::CoreStats::getCurrentHealth()
{
  return current_health_;
}

const float& rpg::CoreStats::getCurrentHealth() const
{
  return current_health_;
}

float& rpg::CoreStats::getCurrentResource()
{
  return current_resource_;
}

const float& rpg::CoreStats::getCurrentResource() const
{
  return current_resource_;
}
