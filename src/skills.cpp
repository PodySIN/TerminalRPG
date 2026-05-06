#include "skills.hpp"
#include "actor.hpp"
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

rpg::KnightSlash::KnightSlash()
{
  skill_name_ = "Knight slash";
  flat_damage_ = 3.0f;
  damage_multiplier_ = 1.0f;
  damage_type_ = DamageType::Physical;
}

void rpg::KnightSlash::doProcessSkill(Actor* owner, Actor* target)
{
  std::pair< float, DamageType > attack =
      owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(attack);
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
