#include "skills.hpp"
#include "actor.hpp"
#include "damage_manager.hpp"
#include "random.hpp"

void rpg::Skill::processSkill(Actor* owner, Actor* target)
{
  doProcessSkill(owner, target);
}

void rpg::Skill::processSkill(Actor* owner, std::vector< Actor* > targets)
{
  for (auto* t : targets)
    doProcessSkill(owner, t);
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
  description_ = "Deal 100% ATK. +5% per lvl";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 0.0f;
}

void rpg::BaseAttack::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::KnightSlash::KnightSlash()
{
  skill_name_ = "Knight Slash";
  description_ = "Deal 120% ATK + 10 flat. +5% per lvl";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 10.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 15.0f;
}

void rpg::KnightSlash::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::ShieldBash::ShieldBash()
{
  skill_name_ = "Shield Bash";
  description_ = "Deal 100% DEF + 15 flat, Stun 1 turn. +5% per lvl";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 15.0f;
  scale_type_ = ScaleType::Defense;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 20.0f;
}

void rpg::ShieldBash::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  target->getEffectManager().addEffect(std::make_unique< StunEffect >(1));
}

rpg::Taunt::Taunt()
{
  skill_name_ = "Taunt";
  description_ = "Force enemy to attack you for 2 turns. +1 turn per 5 lvl";
  resource_cost_ = 15.0f;
  level_scale_ = 0.2f;
}

void rpg::Taunt::doProcessSkill(Actor* owner, Actor* target)
{
  size_t dur = 2 + (size_t)(getSkillLevel() * level_scale_);
  target->getEffectManager().addEffect(std::make_unique< TauntEffect >(dur, owner));
}

rpg::KnightProtection::KnightProtection()
{
  skill_name_ = "Knight Protection";
  description_ = "+30% DEF (+2% per lvl) to ally for 3 turns";
  resource_cost_ = 20.0f;
  level_scale_ = 0.02f;
}

void rpg::KnightProtection::doProcessSkill(Actor*, Actor* target)
{
  float bonus = 1.3f + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< DefenseBuffEffect >(3, bonus));
}

rpg::KnightUltimate1::KnightUltimate1()
{
  skill_name_ = "Divine Wrath";
  description_ = "ULT: Deal 200% ATK + 50 flat, heal 50% dmg. +10% per lvl";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
  resource_cost_ = 60.0f;
}

void rpg::KnightUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  float d = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(d, owner);
  owner->getDamageManager().heal(d * 0.5f);
}

rpg::KnightUltimate2::KnightUltimate2()
{
  skill_name_ = "Last Stand";
  description_ = "ULT: +50% ATK/DEF, +30% SPD (+5% per lvl) for 3 turns";
  resource_cost_ = 50.0f;
  level_scale_ = 0.05f;
}

void rpg::KnightUltimate2::doProcessSkill(Actor* owner, Actor*)
{
  float bonus = 0.5f + getSkillLevel() * level_scale_;
  owner->getEffectManager().addEffect(std::make_unique< DamageBuffEffect >(3, bonus));
  owner->getEffectManager().addEffect(std::make_unique< DefenseBuffEffect >(3, 1.0f + bonus));
  owner->getEffectManager().addEffect(std::make_unique< SpeedBuffEffect >(3, 1.3f + getSkillLevel() * level_scale_));
}

rpg::Fireball::Fireball()
{
  skill_name_ = "Fireball";
  description_ = "Deal 140% ATK + 20 flat, Burn 2 turns. +8% per lvl";
  damage_multiplier_ = 1.4f;
  flat_damage_ = 20.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
  resource_cost_ = 25.0f;
}

void rpg::Fireball::doProcessSkill(Actor* owner, Actor* target)
{
  float d = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(d, owner);
  target->getEffectManager().addEffect(std::make_unique< BurnEffect >(2, d * 0.3f));
}

rpg::IceShard::IceShard()
{
  skill_name_ = "Ice Shard";
  description_ = "Deal 100% ATK, Slow 50% 2 turns. +5% per lvl";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 20.0f;
}

void rpg::IceShard::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  target->getEffectManager().addEffect(std::make_unique< SlowEffect >(2, 0.5f));
}

rpg::LightningStrike::LightningStrike()
{
  skill_name_ = "Lightning Strike";
  description_ = "Deal 150% ATK. +8% per lvl";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
  resource_cost_ = 30.0f;
}

void rpg::LightningStrike::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::ManaShield::ManaShield():
  ManaShield(100.0f, 3)
{}

rpg::ManaShield::ManaShield(float sa, size_t d):
  shield_amount_(sa),
  duration_(d)
{
  skill_name_ = "Mana Shield";
  description_ = "Shield " + std::to_string((int)sa) + " (+20 per lvl) dmg for " + std::to_string(d) + " turns";
  resource_cost_ = 25.0f;
  level_scale_ = 20.0f;
}

void rpg::ManaShield::doProcessSkill(Actor*, Actor* target)
{
  float shield = shield_amount_ + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< ShieldEffect >(duration_, shield));
}

rpg::MageUltimate1::MageUltimate1()
{
  skill_name_ = "Meteor";
  description_ = "ULT: Deal 300% ATK + 50 flat. +15% per lvl";
  damage_multiplier_ = 3.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
  resource_cost_ = 70.0f;
}

void rpg::MageUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::MageUltimate2::MageUltimate2()
{
  skill_name_ = "Time Warp";
  description_ = "ULT: +50% SPD (+5% per lvl) to ally for 3 turns";
  resource_cost_ = 50.0f;
  level_scale_ = 0.05f;
}

void rpg::MageUltimate2::doProcessSkill(Actor*, Actor* target)
{
  float bonus = 1.5f + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< SpeedBuffEffect >(3, bonus));
}

// ==================== ASSASSIN ====================
rpg::Backstab::Backstab()
{
  skill_name_ = "Backstab";
  description_ = "Deal 200% ATK. +10% per lvl";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
  resource_cost_ = 20.0f;
}

void rpg::Backstab::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::PoisonDagger::PoisonDagger()
{
  skill_name_ = "Poison Dagger";
  description_ = "Deal 80% ATK, Bleed 3 turns. +5% per lvl";
  damage_multiplier_ = 0.8f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 15.0f;
  effect_ = std::make_unique< BleedingEffect >();
}

void rpg::PoisonDagger::doProcessSkill(Actor* owner, Actor* target)
{
  if (target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner) && effect_)
    target->getEffectManager().addEffect(effect_->clone());
}

rpg::ShadowStep::ShadowStep()
{
  skill_name_ = "Shadow Step";
  description_ = "+20% SPD (+2% per lvl) for 2 turns";
  resource_cost_ = 15.0f;
  level_scale_ = 0.02f;
}

void rpg::ShadowStep::doProcessSkill(Actor* owner, Actor*)
{
  float bonus = 1.2f + getSkillLevel() * level_scale_;
  owner->getEffectManager().addEffect(std::make_unique< SpeedBuffEffect >(2, bonus));
}

rpg::SpeedBoost::SpeedBoost()
{
  skill_name_ = "Speed Boost";
  description_ = "+20% SPD (+1.5% per lvl) to party for 3 turns";
  resource_cost_ = 20.0f;
  level_scale_ = 0.015f;
}

void rpg::SpeedBoost::doProcessSkill(Actor* owner, Actor*)
{
  float bonus = 1.2f + getSkillLevel() * level_scale_;
  owner->getEffectManager().addEffect(std::make_unique< SpeedBuffEffect >(3, bonus));
}

rpg::AssassinUltimate1::AssassinUltimate1()
{
  skill_name_ = "Death Mark";
  description_ = "ULT: Deal 100% ATK, +50% dmg taken 3 turns. +10% per lvl";
  damage_multiplier_ = 1.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
  resource_cost_ = 60.0f;
}

void rpg::AssassinUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  target->getEffectManager().addEffect(std::make_unique< DamageAmplifyEffect >(3, 1.5f));
}

rpg::AssassinUltimate2::AssassinUltimate2()
{
  skill_name_ = "Assassinate";
  description_ = "ULT: Deal 400% ATK, x2 if <30% HP. +20% per lvl";
  damage_multiplier_ = 4.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
  resource_cost_ = 70.0f;
}

void rpg::AssassinUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  float d = owner->getDamageManager().calculateOutputDamage(this);
  if ((target->getStats().getCurrentHealth() / target->getStats().getHealth().getTotal()) < 0.3f)
    d *= 2.0f;
  target->getDamageManager().handleAttack(d, owner);
}

// ==================== PALADIN ====================
rpg::HolyStrike::HolyStrike()
{
  skill_name_ = "Holy Strike";
  description_ = "Deal 150% ATK, heal 30% dmg. +8% per lvl";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
  resource_cost_ = 15.0f;
}

void rpg::HolyStrike::doProcessSkill(Actor* owner, Actor* target)
{
  float d = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(d, owner);
  owner->getDamageManager().heal(d * 0.3f);
}

rpg::DivineShield::DivineShield()
{
  skill_name_ = "Divine Shield";
  description_ = "Immunity to dmg for 2 turns. +1 turn per 10 lvl";
  resource_cost_ = 30.0f;
  level_scale_ = 0.1f;
}

void rpg::DivineShield::doProcessSkill(Actor*, Actor* target)
{
  size_t dur = 2 + (size_t)(getSkillLevel() * level_scale_);
  target->getEffectManager().addEffect(std::make_unique< InvincibilityEffect >(dur));
}

rpg::HolyLight::HolyLight()
{
  skill_name_ = "Holy Light";
  description_ = "Heal 30% (+2% per lvl) of target's max HP";
  resource_cost_ = 20.0f;
  level_scale_ = 0.02f;
}

void rpg::HolyLight::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.3f + getSkillLevel() * level_scale_;
  target->getDamageManager().heal(target->getStats().getHealth().getTotal() * pct);
}

rpg::AuraOfProtection::AuraOfProtection()
{
  skill_name_ = "Aura of Protection";
  description_ = "+25% DEF (+2% per lvl) to party for 3 turns";
  resource_cost_ = 25.0f;
  level_scale_ = 0.02f;
}

void rpg::AuraOfProtection::doProcessSkill(Actor*, Actor* target)
{
  float bonus = 1.25f + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< DefenseBuffEffect >(3, bonus));
}

rpg::PaladinUltimate1::PaladinUltimate1()
{
  skill_name_ = "Judgment";
  description_ = "ULT: Deal 300% ATK + 50 flat. +15% per lvl";
  damage_multiplier_ = 3.0f;
  flat_damage_ = 50.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
  resource_cost_ = 60.0f;
}

void rpg::PaladinUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::PaladinUltimate2::PaladinUltimate2()
{
  skill_name_ = "Sacrifice";
  description_ = "ULT: Deal 500% ATK, lose 10% HP. +20% per lvl";
  damage_multiplier_ = 5.0f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
  resource_cost_ = 70.0f;
}

void rpg::PaladinUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  owner->getDamageManager().takeDamage(owner->getStats().getHealth().getTotal() * 0.1f);
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::HolySmite::HolySmite()
{
  skill_name_ = "Holy Smite";
  description_ = "Deal 120% ATK + 10 flat. +5% per lvl";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 10.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 15.0f;
}

void rpg::HolySmite::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

// ==================== HEALER ====================
rpg::Heal::Heal()
{
  skill_name_ = "Heal";
  description_ = "Restore 25% (+2% per lvl) max HP";
  resource_cost_ = 20.0f;
  level_scale_ = 0.02f;
}

void rpg::Heal::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.25f + getSkillLevel() * level_scale_;
  target->getDamageManager().heal(target->getStats().getHealth().getTotal() * pct);
}

rpg::Purify::Purify()
{
  skill_name_ = "Purify";
  description_ = "Remove negative effects. +Heal 5% per lvl";
  resource_cost_ = 15.0f;
  level_scale_ = 0.05f;
}

void rpg::Purify::doProcessSkill(Actor*, Actor* target)
{
  target->getEffectManager().removeNegativeEffects();
  target->getDamageManager().heal(target->getStats().getHealth().getTotal() * getSkillLevel() * level_scale_);
}

rpg::SaintUltimate1::SaintUltimate1()
{
  skill_name_ = "Resurrection";
  description_ = "ULT: Revive with 50% (+5% per lvl) HP";
  resource_cost_ = 60.0f;
  level_scale_ = 0.05f;
}

void rpg::SaintUltimate1::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.5f + getSkillLevel() * level_scale_;
  target->getDamageManager().revive(pct);
}

rpg::SaintUltimate2::SaintUltimate2()
{
  skill_name_ = "Divine Intervention";
  description_ = "ULT: Full heal + clear effects";
  resource_cost_ = 70.0f;
}

void rpg::SaintUltimate2::doProcessSkill(Actor*, Actor* target)
{
  target->getDamageManager().heal(target->getStats().getHealth().getTotal());
  target->getEffectManager().removeNegativeEffects();
}

// ==================== PRIEST ====================
rpg::Smite::Smite()
{
  skill_name_ = "Smite";
  description_ = "Deal 110% ATK. +5% per lvl";
  damage_multiplier_ = 1.1f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 10.0f;
}

void rpg::Smite::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::HealPrayer::HealPrayer()
{
  skill_name_ = "Heal Prayer";
  description_ = "Heal 20% (+1.5% per lvl) of target's max HP";
  resource_cost_ = 25.0f;
  level_scale_ = 0.015f;
}

void rpg::HealPrayer::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.2f + getSkillLevel() * level_scale_;
  target->getDamageManager().heal(target->getStats().getHealth().getTotal() * pct);
}

rpg::ShieldOfFaith::ShieldOfFaith()
{
  skill_name_ = "Shield of Faith";
  description_ = "Shield 30% (+3% per lvl) max HP for 2 turns";
  resource_cost_ = 20.0f;
  level_scale_ = 0.03f;
}

void rpg::ShieldOfFaith::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.3f + getSkillLevel() * level_scale_;
  target->getStats().addShield(target->getStats().getHealth().getTotal() * pct);
}

rpg::Renew::Renew()
{
  skill_name_ = "Renew";
  description_ = "Heal 10% (+1% per lvl) max HP/turn for 3 turns";
  resource_cost_ = 20.0f;
  level_scale_ = 0.01f;
}

void rpg::Renew::doProcessSkill(Actor*, Actor* target)
{
  float pct = 0.1f + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< HealOverTimeEffect >(3, pct));
}

rpg::PriestUltimate1::PriestUltimate1()
{
  skill_name_ = "Mass Resurrection";
  description_ = "ULT: Revive with 30% (+4% per lvl) HP";
  resource_cost_ = 60.0f;
  level_scale_ = 0.04f;
}

void rpg::PriestUltimate1::doProcessSkill(Actor*, Actor* target)
{
  if (target->getStats().getIsDead()) {
    float pct = 0.3f + getSkillLevel() * level_scale_;
    target->getDamageManager().revive(pct);
  }
}

rpg::PriestUltimate2::PriestUltimate2()
{
  skill_name_ = "Miracle";
  description_ = "ULT: Full heal + immunity 1 turn";
  resource_cost_ = 70.0f;
}

void rpg::PriestUltimate2::doProcessSkill(Actor*, Actor* target)
{
  target->getDamageManager().heal(target->getStats().getHealth().getTotal());
  target->getEffectManager().addEffect(std::make_unique< InvincibilityEffect >(1));
}

// ==================== ARCHER ====================
rpg::QuickShot::QuickShot()
{
  skill_name_ = "Quick Shot";
  description_ = "Deal 85% ATK, 25% chance x2. +5% per lvl";
  damage_multiplier_ = 0.85f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.05f;
  resource_cost_ = 10.0f;
}

void rpg::QuickShot::doProcessSkill(Actor* owner, Actor* target)
{
  float d = owner->getDamageManager().calculateOutputDamage(this);
  target->getDamageManager().handleAttack(d, owner);
  if (Random::getFloat(0.0f, 1.0f) < 0.25f)
    target->getDamageManager().handleAttack(d, owner);
}

rpg::AimedShot::AimedShot()
{
  skill_name_ = "Aimed Shot";
  description_ = "Deal 150% ATK, +25% Crit. +10% per lvl";
  damage_multiplier_ = 1.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.1f;
  resource_cost_ = 20.0f;
}

void rpg::AimedShot::doProcessSkill(Actor* owner, Actor* target)
{
  float old = owner->getStats().getCritChance().getBase();
  owner->getStats().getCritChance().addBase(0.25f);
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  owner->getStats().getCritChance().setBase(old);
}

rpg::PiercingArrow::PiercingArrow()
{
  skill_name_ = "Piercing Arrow";
  description_ = "Deal 120% ATK, ignore 30% DEF. +8% per lvl";
  damage_multiplier_ = 1.2f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.08f;
  resource_cost_ = 15.0f;
}

void rpg::PiercingArrow::doProcessSkill(Actor* owner, Actor* target)
{
  target->getStats().getDefense().addMultiply(-0.3f);
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  target->getStats().getDefense().addMultiply(0.3f);
}

rpg::HawkEye::HawkEye()
{
  skill_name_ = "Hawk Eye";
  description_ = "+15% Crit (+2% per lvl) for 3 turns";
  resource_cost_ = 20.0f;
  level_scale_ = 0.02f;
}

void rpg::HawkEye::doProcessSkill(Actor* owner, Actor*)
{
  float bonus = 0.15f + getSkillLevel() * level_scale_;
  owner->getEffectManager().addEffect(std::make_unique< CritChanceBuffEffect >(3, bonus));
}

rpg::ArcherUltimate1::ArcherUltimate1()
{
  skill_name_ = "Rain of Arrows";
  description_ = "ULT: Deal 200% ATK + 30 flat. +15% per lvl";
  damage_multiplier_ = 2.0f;
  flat_damage_ = 30.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.15f;
  resource_cost_ = 60.0f;
}

void rpg::ArcherUltimate1::doProcessSkill(Actor* owner, Actor* target)
{
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
}

rpg::ArcherUltimate2::ArcherUltimate2()
{
  skill_name_ = "Perfect Shot";
  description_ = "ULT: Deal 350% ATK, 100% Crit. +20% per lvl";
  damage_multiplier_ = 3.5f;
  flat_damage_ = 0.0f;
  scale_type_ = ScaleType::Damage;
  level_damage_scale_ = 0.2f;
  resource_cost_ = 70.0f;
}

void rpg::ArcherUltimate2::doProcessSkill(Actor* owner, Actor* target)
{
  float old = owner->getStats().getCritChance().getBase();
  owner->getStats().getCritChance().setBase(1.0f);
  target->getDamageManager().handleAttack(owner->getDamageManager().calculateOutputDamage(this), owner);
  owner->getStats().getCritChance().setBase(old);
}

// ==================== OTHER ====================
rpg::DemonMark::DemonMark()
{
  skill_name_ = "Demon Mark";
  description_ = "+30% (+2% per lvl) dmg taken for 3 turns";
  resource_cost_ = 15.0f;
  level_scale_ = 0.02f;
}

void rpg::DemonMark::doProcessSkill(Actor*, Actor* target)
{
  float bonus = 1.3f + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< DamageAmplifyEffect >(3, bonus));
}

rpg::Curse::Curse():
  Curse(0.3f, 3)
{}

rpg::Curse::Curse(float dm, int d):
  damage_multiplier_(dm),
  duration_(d)
{
  skill_name_ = "Curse";
  description_ =
      "+" + std::to_string((int)(dm * 100)) + "% (+3% per lvl) dmg taken for " + std::to_string(d) + " turns";
  resource_cost_ = 15.0f;
  level_scale_ = 0.03f;
}

void rpg::Curse::doProcessSkill(Actor*, Actor* target)
{
  float bonus = 1.0f + damage_multiplier_ + getSkillLevel() * level_scale_;
  target->getEffectManager().addEffect(std::make_unique< DamageAmplifyEffect >(duration_, bonus));
}
