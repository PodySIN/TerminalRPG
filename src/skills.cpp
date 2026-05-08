#include "skills.hpp"
#include "actor.hpp"
#include "effects.hpp"
#include "types.hpp"

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

rpg::KnightSlash::KnightSlash()
{
  skill_name_ = "Knight slash";
  flat_damage_ = 3.0f;
  scale_type_ = ScaleType::Damage;
  damage_multiplier_ = 1.0f;
  damage_type_ = DamageType::Physical;
}

void rpg::KnightSlash::doProcessSkill(Actor* owner, Actor* target)
{
  std::pair< float, DamageType > attack =
      owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(attack);
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
  bool isHit = target->getDamageManager().handleAttack(attack);
  if (isHit) {
    auto effect = effect_->clone();
    target->getEffectManager().addEffect(std::move(effect));
  }
}
