#include "damage_manager.hpp"
#include "actor.hpp"
#include "core_stats.hpp"
#include "random.hpp"

rpg::DamageManager::DamageManager(Actor* owner) : owner_(owner)
{
}

float rpg::DamageManager::calculateDodgeChance()
{
  float evasion = owner_->getStats().getEvasion().getTotal();
  return evasion / (evasion + 100.0f);
}

bool rpg::DamageManager::handleAttack(std::pair< float, DamageType >& attack)
{
  if (Random::getFloat(0.0f, 1.0f) < calculateDodgeChance()) {
    return false;
  }
  float damage = calculateInputDamage(attack);
  takeDamage(damage);
  return true;
}

float rpg::DamageManager::calculateInputDamage(
    std::pair< float, DamageType >& attack)
{
  float damage = attack.first;
  damage -= owner_->getStats().getBlockDamage().getBase();
  switch (attack.second) {
    case DamageType::Physical:
      damage *= (1 - owner_->getStats().calculatePhysicalResistance());
      break;
    case DamageType::Magic:
      damage *= (1 - owner_->getStats().getMagicResistance().getBase());
      break;
    case DamageType::Pure:
      break;
  }
  damage *= (1 - owner_->getStats().getDamageReduction().getBase());
  return damage;
}

std::pair< float, rpg::DamageType >
rpg::DamageManager::calculateOutputDamage(AttackSkill* skill)
{
  float damage = 0.0f;
  switch (skill->getScaleType()) {
    case ScaleType::Damage:
      damage = owner_->getStats().getDamage().getTotal();
      break;
    case ScaleType::Health:
      damage = owner_->getStats().getHealth().getTotal();
      break;
    case ScaleType::Defense:
      damage = owner_->getStats().getDefense().getTotal();
      break;
    case ScaleType::Resource:
      damage = owner_->getStats().getResource().getTotal();
      break;
  }
  damage *= skill->getDamageMultiplier();
  damage += skill->getFlatDamage();
  if (Random::getFloat(0.0f, 1.0f) <
      owner_->getStats().getCritChance().getBase()) {
    damage *= owner_->getStats().getCritDamage().getBase();
  }
  damage *= owner_->getStats().getDamageBonus().getBase();
  return {damage, skill->getDamageType()};
}

void rpg::DamageManager::takeDamage(float damage)
{
  if (owner_->getStats().getCurrentHealth() - damage <= 0) {
    owner_->getStats().getCurrentHealth() = 0;
    owner_->die();
    return;
  }
  owner_->getStats().getCurrentHealth() -= damage;
}
