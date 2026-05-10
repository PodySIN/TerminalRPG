#include "skills.hpp"
#include "actor.hpp"
#include "effects.hpp"
#include "types.hpp"
#include <memory>

void rpg::Skill::processSkill(Actor* owner, Actor* target)
{
  doProcessSkill(owner, target);
}

void rpg::Skill::processSkill(Actor* owner, std::vector< Actor* > targets)
{
  for (size_t i = 0; i < targets.size(); i++) {
    doProcessSkill(owner, targets[i]);
  }
}

std::string rpg::Skill::getSkillName() const
{
  return skill_name_;
}

rpg::ScaleType rpg::AttackSkill::getScaleType() const
{
  return scale_type_;
}

float rpg::AttackSkill::getDamageMultiplier() const
{
  return damage_multiplier_;
}

float rpg::AttackSkill::getFlatDamage() const
{
  return flat_damage_;
}

rpg::DamageType rpg::AttackSkill::getDamageType() const
{
  return damage_type_;
}

rpg::BaseAttack::BaseAttack(float damage_multiplier, float flat_damage,
                            DamageType damage_type, ScaleType scale_type)
{
  skill_name_ = "Base attack";
  flat_damage_ = flat_damage;
  scale_type_ = scale_type;
  damage_multiplier_ = damage_multiplier;
  damage_type_ = damage_type;
}

rpg::BaseAttack::BaseAttack() :
  rpg::BaseAttack(1.0f, 0.0f, DamageType::Physical, ScaleType::Damage)
{
}

void rpg::BaseAttack::doProcessSkill(Actor* owner, Actor* target)
{
  std::pair< float, DamageType > attack =
      owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(attack, owner);
}

rpg::SavageSlash::SavageSlash()
{
  skill_name_ = "Savage Slash";
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  damage_multiplier_ = 1.5f;
  damage_type_ = DamageType::Physical;
  effect_ = std::make_unique< BleedingEffect >();
}

void rpg::SavageSlash::doProcessSkill(Actor* owner, Actor* target)
{
  std::pair< float, DamageType > attack =
      owner->getDamageManager().calculateOutputDamage(this);
  bool isHit = target->getDamageManager().handleAttack(attack, owner);
  if (isHit) {
    auto effect = effect_->clone();
    target->getEffectManager().addEffect(std::move(effect));
  }
}

rpg::GuardianShield::GuardianShield()
{
  skill_name_ = "Guardian Shield";
  parry_effect_ = std::make_unique< ParryEffect >();
  block_damage_buff_effect_ = std::make_unique< BlockDamageBuffEffect >();
}

void rpg::GuardianShield::doProcessSkill(Actor*, Actor* target)
{
  auto parry_effect = parry_effect_->clone();
  auto block_damage_buff_effect = block_damage_buff_effect_->clone();
  target->getEffectManager().addEffect(std::move(block_damage_buff_effect));
  target->getEffectManager().addEffect(std::move(parry_effect));
}

rpg::ParryAttack::ParryAttack(float damage_multiplier, ScaleType scale_type,
                              DamageType damage_type)
{
  skill_name_ = "Parry attack";
  damage_multiplier_ = damage_multiplier;
  flat_damage_ = 0.0f;
  scale_type_ = scale_type;
  damage_type_ = damage_type;
}

rpg::ParryAttack::ParryAttack() :
  ParryAttack(0.75f, ScaleType::Damage, DamageType::Physical)
{
}

void rpg::ParryAttack::doProcessSkill(rpg::Actor* owner, Actor* target)
{
  std::pair< float, DamageType > attack =
      owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(attack, owner);
}
