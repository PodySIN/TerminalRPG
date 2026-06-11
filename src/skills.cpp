#include "skills.hpp"
#include "actor.hpp"
#include "damage_manager.hpp"

void rpg::Skill::processSkill(Actor* owner, Actor* target)
{
  doProcessSkill(owner, target);
}

void rpg::Skill::processSkill(Actor* owner, std::vector< Actor* > targets)
{
  for (auto* target : targets) {
    doProcessSkill(owner, target);
  }
}

std::string rpg::Skill::getSkillName() const
{
  return skill_name_;
}

std::string rpg::Skill::getDescription() const
{
  return description_;
}

void rpg::Skill::addSkillLevel(size_t value)
{
  skill_level_ += value;
}

size_t rpg::Skill::getSkillLevel() const
{
  return skill_level_;
}

float rpg::AttackSkill::getDamageMultiplier() const
{
  return damage_multiplier_ + (getSkillLevel() * level_damage_scale_);
}

float rpg::AttackSkill::getFlatDamage() const
{
  return flat_damage_;
}

rpg::ScaleType rpg::AttackSkill::getScaleType() const
{
  return scale_type_;
}

float rpg::AttackSkill::getLevelDamageScale() const
{
  return level_damage_scale_;
}

rpg::BaseAttack::BaseAttack()
{
  skill_name_ = "Base Attack";
  description_ = "Deal 100% damage";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

rpg::BaseAttack::BaseAttack(float damage_multiplier, float flat_damage, ScaleType scale_type)
{
  skill_name_ = "Base Attack";
  description_ =
      "Deal " + std::to_string((int)(damage_multiplier * 100)) + "% damage + " + std::to_string((int)flat_damage);
  damage_multiplier_ = damage_multiplier;
  flat_damage_ = flat_damage;
  scale_type_ = scale_type;
  level_damage_scale_ = 0.05f;
}

void rpg::BaseAttack::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::KnightSlash::KnightSlash()
{
  skill_name_ = "Knight Slash";
  description_ = "Deal 120% damage + 10 flat damage";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 10.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

void rpg::KnightSlash::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::ShieldBash::ShieldBash()
{
  skill_name_ = "Shield Bash";
  description_ = "Deal 100% defense damage + 15 flat damage, stun for 1 turn";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 15.0f;
  scale_type_ = ScaleType::Defense;
  level_damage_scale_ = 0.05f;
}

void rpg::ShieldBash::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  auto stun = std::make_unique< StunEffect >(1);
  target->getEffectManager().addEffect(std::move(stun));
}

rpg::Taunt::Taunt()
{
  skill_name_ = "Taunt";
  description_ = "Force enemy to attack you for 2 turns";
}

void rpg::Taunt::doProcessSkill(Actor*, Actor* target)
{
  auto taunt = std::make_unique< TauntEffect >(2);
  target->getEffectManager().addEffect(std::move(taunt));
}

rpg::KnightProtection::KnightProtection()
{
  skill_name_ = "Knight Protection";
  description_ = "Increase ally defense by 30% for 3 turns";
}

void rpg::KnightProtection::doProcessSkill(Actor*, Actor* target)
{
  auto buff = std::make_unique< DefenseBuffEffect >(3, 1.3f);
  target->getEffectManager().addEffect(std::move(buff));
}

rpg::KnightUltimate1::KnightUltimate1()
{
  skill_name_ = "Divine Wrath";
  description_ = "ULTIMATE: Deal 200% damage + 50 flat, heal for 50% of damage dealt";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
}

void rpg::KnightUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  owner->getDamageManager().heal(damage * 0.5f);
}

rpg::KnightUltimate2::KnightUltimate2()
{
  skill_name_ = "Last Stand";
  description_ = "ULTIMATE: Increase all stats by 50% for 3 turns";
}

void rpg::KnightUltimate2::doProcessSkill(Actor* owner, Actor*)
{
  auto buff = std::make_unique< DamageBuffEffect >(3, 0.5f);
  owner->getEffectManager().addEffect(std::move(buff));
  owner->getStats().getDefense().addMultiply(0.5f);
  owner->getStats().getSpeed().addMultiply(0.3f);
}

rpg::Fireball::Fireball()
{
  skill_name_ = "Fireball";
  description_ = "Deal 140% damage + 20 flat, burn for 2 turns";
  damage_multiplier_ = 1.4f;
  flat_damage_ = 20.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
}

void rpg::Fireball::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);

  auto burn = std::make_unique< BurnEffect >(2, damage * 0.3f);
  target->getEffectManager().addEffect(std::move(burn));
}

rpg::IceShard::IceShard()
{
  skill_name_ = "Ice Shard";
  description_ = "Deal 100% damage, slow by 50% for 2 turns";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

void rpg::IceShard::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  auto slow = std::make_unique< SlowEffect >(2, 0.5f);
  target->getEffectManager().addEffect(std::move(slow));
}

rpg::LightningStrike::LightningStrike()
{
  skill_name_ = "Lightning Strike";
  description_ = "Deal 150% damage";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
}

void rpg::LightningStrike::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::ManaShield::ManaShield():
  ManaShield(100.0f, 3) // 100 щита на 3 хода
{}

rpg::ManaShield::ManaShield(float shield_amount, size_t duration):
  shield_amount_(shield_amount),
  duration_(duration)
{
  skill_name_ = "Mana Shield";
  description_ = "Grant a shield that absorbs " + std::to_string((int)shield_amount_) + " damage for " +
                 std::to_string(duration_) + " turns";
}

void rpg::ManaShield::doProcessSkill(Actor*, Actor* target)
{
  auto shield = std::make_unique< ShieldEffect >(duration_, shield_amount_);
  target->getEffectManager().addEffect(std::move(shield));
}

rpg::MageUltimate1::MageUltimate1()
{
  skill_name_ = "Meteor";
  description_ = "ULTIMATE: Deal 300% AoE damage + 50 flat";
  damage_multiplier_ = 3.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
}

void rpg::MageUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::MageUltimate2::MageUltimate2()
{
  skill_name_ = "Time Warp";
  description_ = "ULTIMATE: Increase ally speed by 50% for 2 turns, grant extra turn";
}

void rpg::MageUltimate2::doProcessSkill(Actor*, Actor* target)
{
  auto buff = std::make_unique< SpeedBuffEffect >(2, 1.5f);
  target->getEffectManager().addEffect(std::move(buff));
}

rpg::Backstab::Backstab()
{
  skill_name_ = "Backstab";
  description_ = "Deal 200% damage from behind";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
}

void rpg::Backstab::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::PoisonDagger::PoisonDagger()
{
  skill_name_ = "Poison Dagger";
  description_ = "Deal 80% damage, poison for 3 turns";
  damage_multiplier_ = 0.8f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  effect_ = std::make_unique< BleedingEffect >();
}

void rpg::PoisonDagger::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  bool hit = target->getDamageManager().handleAttack(damage, owner);
  if (hit && effect_) {
    target->getEffectManager().addEffect(effect_->clone());
  }
}

rpg::ShadowStep::ShadowStep()
{
  skill_name_ = "Shadow Step";
  description_ = "Increase evasion for 2 turns";
}

void rpg::ShadowStep::doProcessSkill(Actor* owner, Actor*)
{
  auto buff = std::make_unique< SpeedBuffEffect >(2, 1.2f);
  owner->getEffectManager().addEffect(std::move(buff));
}

rpg::SpeedBoost::SpeedBoost()
{
  skill_name_ = "Speed Boost";
  description_ = "Increase party speed for 3 turns";
}

void rpg::SpeedBoost::doProcessSkill(Actor* owner, Actor*)
{
  auto buff = std::make_unique< SpeedBuffEffect >(3, 1.2f);
  owner->getEffectManager().addEffect(std::move(buff));
}

rpg::AssassinUltimate1::AssassinUltimate1()
{
  skill_name_ = "Death Mark";
  description_ = "ULTIMATE: Target takes 50% more damage for 3 turns";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
}

void rpg::AssassinUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  auto mark = std::make_unique< DamageAmplifyEffect >(3, 1.5f);
  target->getEffectManager().addEffect(std::move(mark));
}

rpg::AssassinUltimate2::AssassinUltimate2()
{
  skill_name_ = "Assassinate";
  description_ = "ULTIMATE: Deal 400% damage, double damage if target below 30% HP";
  damage_multiplier_ = 4.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
}

void rpg::AssassinUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::HolyStrike::HolyStrike()
{
  skill_name_ = "Holy Strike";
  description_ = "Deal 150% holy damage, heal for 30% of damage dealt";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
}

void rpg::HolyStrike::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  owner->getDamageManager().heal(damage * 0.3f);
}

rpg::DivineShield::DivineShield()
{
  skill_name_ = "Divine Shield";
  description_ = "Grant immunity to all damage for 2 turns";
}

void rpg::DivineShield::doProcessSkill(Actor*, Actor* target)
{
  auto shield = std::make_unique< InvincibilityEffect >(2);
  target->getEffectManager().addEffect(std::move(shield));
}

rpg::HolyLight::HolyLight()
{
  skill_name_ = "Holy Light";
  description_ = "Heal ally for 30% of max HP";
}

void rpg::HolyLight::doProcessSkill(Actor*, Actor* target)
{
  float heal = target->getStats().getHealth().getTotal() * 0.3f;
  target->getDamageManager().heal(heal);
}

rpg::AuraOfProtection::AuraOfProtection()
{
  skill_name_ = "Aura of Protection";
  description_ = "Increase party defense by 25% for 3 turns";
}

void rpg::AuraOfProtection::doProcessSkill(Actor*, Actor* target)
{
  auto buff = std::make_unique< DefenseBuffEffect >(3, 1.25f);
  target->getEffectManager().addEffect(std::move(buff));
}

rpg::PaladinUltimate1::PaladinUltimate1()
{
  skill_name_ = "Judgment";
  description_ = "ULTIMATE: Deal 300% holy damage to all enemies";
  damage_multiplier_ = 3.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
}

void rpg::PaladinUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::PaladinUltimate2::PaladinUltimate2()
{
  skill_name_ = "Sacrifice";
  description_ = "ULTIMATE: Deal 500% damage, sacrifice 10% of max HP";
  damage_multiplier_ = 5.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
}

void rpg::PaladinUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  float hp_cost = owner->getStats().getHealth().getTotal() * 0.1f;
  owner->getDamageManager().takeDamage(hp_cost);

  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::HolySmite::HolySmite()
{
  skill_name_ = "Holy Smite";
  description_ = "Deal 120% holy damage + 10 flat";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 10.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

void rpg::HolySmite::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::Heal::Heal()
{
  skill_name_ = "Heal";
  description_ = "Restore 25% of target's max HP";
}

void rpg::Heal::doProcessSkill(Actor*, Actor* target)
{
  float heal = target->getStats().getHealth().getTotal() * 0.25f;
  target->getDamageManager().heal(heal);
}

rpg::Purify::Purify()
{
  skill_name_ = "Purify";
  description_ = "Remove all negative effects from target";
}

void rpg::Purify::doProcessSkill(Actor*, Actor* target)
{
  target->getEffectManager().removeNegativeEffects();
}

rpg::SaintUltimate1::SaintUltimate1()
{
  skill_name_ = "Resurrection";
  description_ = "ULTIMATE: Revive fallen ally with 50% HP";
}

void rpg::SaintUltimate1::doProcessSkill(Actor*, Actor* target)
{
  target->getDamageManager().revive(0.5f);
}

rpg::SaintUltimate2::SaintUltimate2()
{
  skill_name_ = "Divine Intervention";
  description_ = "ULTIMATE: Fully heal all allies, remove all negative effects";
}

void rpg::SaintUltimate2::doProcessSkill(Actor*, Actor* target)
{

  target->getDamageManager().heal(target->getStats().getHealth().getTotal());
  target->getEffectManager().removeNegativeEffects();
}

rpg::Smite::Smite()
{
  skill_name_ = "Smite";
  description_ = "Deal 110% holy damage";
  damage_multiplier_ = 1.1f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

void rpg::Smite::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::HealPrayer::HealPrayer()
{
  skill_name_ = "Heal Prayer";
  description_ = "Restore 20% of all allies' max HP";
}

void rpg::HealPrayer::doProcessSkill(Actor*, Actor* target)
{
  target->getDamageManager().heal(target->getStats().getHealth().getTotal() * 0.2f);
}

rpg::ShieldOfFaith::ShieldOfFaith()
{
  skill_name_ = "Shield of Faith";
  description_ = "Grant shield equal to 30% of max HP for 2 turns";
}

void rpg::ShieldOfFaith::doProcessSkill(Actor*, Actor* target)
{
  float shield = target->getStats().getHealth().getTotal() * 0.3f;
  target->getStats().addShield(shield);
}

rpg::Renew::Renew()
{
  skill_name_ = "Renew";
  description_ = "Heal over time: restore 10% max HP per turn for 3 turns";
}

void rpg::Renew::doProcessSkill(Actor*, Actor* target)
{
  auto hot = std::make_unique< HealOverTimeEffect >(3, 0.1f);
  target->getEffectManager().addEffect(std::move(hot));
}

rpg::PriestUltimate1::PriestUltimate1()
{
  skill_name_ = "Mass Resurrection";
  description_ = "ULTIMATE: Revive all fallen allies with 30% HP";
}

void rpg::PriestUltimate1::doProcessSkill(Actor*, Actor* target)
{
  if (target->getStats().getIsDead()) {
    target->getDamageManager().revive(0.3f);
  }
}

rpg::PriestUltimate2::PriestUltimate2()
{
  skill_name_ = "Miracle";
  description_ = "ULTIMATE: Fully heal and grant immunity for 1 turn to all allies";
}

void rpg::PriestUltimate2::doProcessSkill(Actor*, Actor* target)
{
  target->getDamageManager().heal(target->getStats().getHealth().getTotal());
  auto immunity = std::make_unique< InvincibilityEffect >(1);
  target->getEffectManager().addEffect(std::move(immunity));
}

rpg::QuickShot::QuickShot()
{
  skill_name_ = "Quick Shot";
  description_ = "Deal 85% damage, 25% chance to attack twice";
  damage_multiplier_ = 0.85f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
}

void rpg::QuickShot::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::AimedShot::AimedShot()
{
  skill_name_ = "Aimed Shot";
  description_ = "Deal 150% damage, +25% crit chance";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
}

void rpg::AimedShot::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::PiercingArrow::PiercingArrow()
{
  skill_name_ = "Piercing Arrow";
  description_ = "Deal 120% damage, ignores 30% defense";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
}

void rpg::PiercingArrow::doProcessSkill(Actor* owner, Actor* target)
{
  target->getStats().getDefense().addMultiply(-0.3f);
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  target->getStats().getDefense().addMultiply(0.3f);
}

rpg::HawkEye::HawkEye()
{
  skill_name_ = "Hawk Eye";
  description_ = "Increase crit chance by 15% for 3 turns";
}

void rpg::HawkEye::doProcessSkill(Actor* owner, Actor*)
{
  owner->getStats().getCritChance().addBase(0.15f);
  auto buff = std::make_unique< DamageBuffEffect >(3, 0.15f);
  owner->getEffectManager().addEffect(std::move(buff));
}

rpg::ArcherUltimate1::ArcherUltimate1()
{
  skill_name_ = "Rain of Arrows";
  description_ = "ULTIMATE: Deal 200% AoE damage to all enemies";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 30.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
}

void rpg::ArcherUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
}

rpg::ArcherUltimate2::ArcherUltimate2()
{
  skill_name_ = "Perfect Shot";
  description_ = "ULTIMATE: Deal 350% damage, guaranteed crit";
  damage_multiplier_ = 3.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
}

void rpg::ArcherUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  float old_crit = owner->getStats().getCritChance().getBase();
  owner->getStats().getCritChance().setBase(1.0f);
  float damage = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(damage, owner);
  owner->getStats().getCritChance().setBase(old_crit);
}

rpg::DemonMark::DemonMark()
{
  skill_name_ = "Demon Mark";
  description_ = "Mark enemy, increasing damage taken by 30% for 3 turns";
}

void rpg::DemonMark::doProcessSkill(Actor*, Actor* target)
{
  auto mark = std::make_unique< DamageAmplifyEffect >(3, 1.3f);
  target->getEffectManager().addEffect(std::move(mark));
}

rpg::Curse::Curse():
  Curse(0.3f, 3)
{}

rpg::Curse::Curse(float damage_multiplier, int duration):
  damage_multiplier_(damage_multiplier),
  duration_(duration)
{
  skill_name_ = "Curse";
  description_ = "Curse the enemy, increasing damage taken by " + std::to_string((int)(damage_multiplier * 100)) +
                 "% for " + std::to_string(duration) + " turns";
}

void rpg::Curse::doProcessSkill(Actor*, Actor* target)
{
  auto curse_effect = std::make_unique< DamageAmplifyEffect >(duration_, 1.0f + damage_multiplier_);
  target->getEffectManager().addEffect(std::move(curse_effect));
}
