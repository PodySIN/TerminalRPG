#include "damage_manager.hpp"
#include "actor.hpp"
#include "core_stats.hpp"
#include "effects.hpp"
#include "random.hpp"

rpg::DamageManager::DamageManager(rpg::Actor* owner):
  owner_(owner)
{}

bool rpg::DamageManager::handleAttack(float attack, rpg::Actor* attacker)
{
  float damage = calculateInputDamage(attack);
  auto effect = owner_->getEffectManager().getEffect(rpg::EffectType::Parry);
  if (effect) {
    rpg::ParryEffect* parry = dynamic_cast< rpg::ParryEffect* >(effect);
    if (parry && rpg::Random::getFloat(0.0f, 1.0f) < parry->getParryChance()) {
      if (owner_ != attacker) {
        owner_->getSkillManager().useSkill(0, attacker);
      }
    }
  }
  takeDamage(damage);
  return true;
}

float rpg::DamageManager::calculateInputDamage(float attack)
{
  float damage = attack;
  float phys_res = owner_->getStats().calculatePhysicalResistance();
  float damage_reduction = owner_->getStats().getDamageReduction().getBase();
  damage *= 1.0f - phys_res;
  damage *= 1.0f - damage_reduction;
  if (damage < 0) {
    damage *= (-1);
  }
  return damage;
}

float rpg::DamageManager::calculateOutputDamage(rpg::AttackSkill* skill)
{
  float damage = 0.0f;
  switch (skill->getScaleType()) {
    case rpg::ScaleType::Damage:
      damage = owner_->getStats().getDamage().getTotal();
      break;
    case rpg::ScaleType::Health:
      damage = owner_->getStats().getHealth().getTotal();
      break;
    case rpg::ScaleType::Defense:
      damage = owner_->getStats().getDefense().getTotal();
      break;
    case rpg::ScaleType::Resource:
      damage = owner_->getStats().getResource().getTotal();
      break;
  }
  damage *= skill->getDamageMultiplier();
  damage += skill->getFlatDamage();
  if (rpg::Random::getFloat(0.0f, 1.0f) < owner_->getStats().getCritChance().getBase()) {
    damage *= owner_->getStats().getCritDamage().getBase();
  }
  float damage_bonus = owner_->getStats().getDamageBonus().getBase();
  if (damage_bonus < 0) {
    return 0.0f;
  }
  damage *= damage_bonus;
  return damage;
}

void rpg::DamageManager::takeDamage(float damage)
{
  if (damage <= 0)
    return;
  auto& stats = owner_->getStats();
  if (stats.isInvincible()) {
    return;
  }
  if (stats.getShield() > 0) {
    float shield_absorb = std::min(damage, stats.getShield());
    stats.setShield(stats.getShield() - shield_absorb);
    damage -= shield_absorb;
  }
  if (damage <= 0) {
    return;
  }
  damage *= (1 - stats.getDamageReduction().getBase());
  if (stats.getCurrentHealth() - damage <= 0) {
    stats.getCurrentHealth() = 0;
    owner_->die();
  } else {
    stats.getCurrentHealth() -= damage;
  }
}

void rpg::DamageManager::heal(float amount)
{
  if (amount <= 0)
    return;

  auto& stats = owner_->getStats();
  float max_health = stats.getHealth().getTotal();
  float new_health = stats.getCurrentHealth() + amount;
  stats.getCurrentHealth() = std::min(max_health, new_health);
}

void rpg::DamageManager::revive(float percent)
{
  auto& stats = owner_->getStats();
  float max_health = stats.getHealth().getTotal();
  stats.getCurrentHealth() = max_health * percent;
  stats.setInvincible(false);
  stats.setStunned(false);
}
