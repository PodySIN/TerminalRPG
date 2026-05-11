#include "damage_manager.hpp"
#include "actor.hpp"
#include "core_stats.hpp"
#include "effects.hpp"
#include "random.hpp"
#include <memory>

rpg::DamageManager::DamageManager(Actor* owner) : owner_(owner)
{
}

bool rpg::DamageManager::handleAttack(float attack, Actor* attacker)
{
  float damage = calculateInputDamage(attack);
  std::unique_ptr< Effect > effect =
      owner_->getEffectManager().isActorHasEffect(EffectType::Parry);
  if (effect) {
    ParryEffect* parry_effect = dynamic_cast< ParryEffect* >(effect.get());
    if (parry_effect) {
      if (Random::getFloat(0.0f, 1.0f) < parry_effect->getParryChance()) {
        if (owner_ != attacker) {
          owner_->getSkillManager().useSkill(0, attacker);
        }
      }
    }
  }
  takeDamage(damage);
  return true;
}

float rpg::DamageManager::calculateInputDamage(float attack)
{
  float damage = attack;
  damage *= (1 - owner_->getStats().calculatePhysicalResistance());
  damage *= (1 - owner_->getStats().getDamageReduction().getBase());
  return damage;
}

float rpg::DamageManager::calculateOutputDamage(AttackSkill* skill)
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
  return damage;
}

void rpg::DamageManager::takeDamage(float damage)
{
  if (damage <= 0) {
    return;
  }
  if (owner_->getStats().getCurrentHealth() - damage <= 0) {
    owner_->getStats().getCurrentHealth() = 0;
    owner_->die();
    return;
  }
  owner_->getStats().getCurrentHealth() -= damage;
}
