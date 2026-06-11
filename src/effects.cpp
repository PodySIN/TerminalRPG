#include "effects.hpp"
#include <memory>
#include "actor.hpp"

rpg::Effect::Effect(rpg::EffectType type, size_t duration):
  type_(type),
  duration_(duration)
{}

void rpg::Effect::onApply(rpg::Actor* owner)
{
  doOnApply(owner);
}

void rpg::Effect::onTick(rpg::Actor* owner)
{
  doOnTick(owner);
  reduceDuration();
}

void rpg::Effect::onRemove(rpg::Actor* owner)
{
  doOnRemove(owner);
}

bool rpg::Effect::isExpired() const
{
  return duration_ == 0;
}

void rpg::Effect::reduceDuration()
{
  if (duration_ > 0)
    duration_--;
}

rpg::EffectType rpg::Effect::getEffectType() const
{
  return type_;
}

float rpg::Effect::getDuration() const
{
  return static_cast< float >(duration_);
}

void rpg::Effect::setDuration(size_t value)
{
  duration_ = value;
}

void rpg::Effect::doOnStack(rpg::Effect*)
{}

rpg::NailingEffect::NailingEffect(size_t duration, float speed_reduction):
  rpg::Effect(rpg::EffectType::Nailing, duration),
  speed_reduction_(speed_reduction)
{}

rpg::NailingEffect::NailingEffect():
  rpg::NailingEffect(3, 20.0f)
{}

bool rpg::NailingEffect::isStackable() const
{
  return true;
}

void rpg::NailingEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::NailingEffect::clone() const
{
  return std::make_unique< rpg::NailingEffect >(*this);
}

void rpg::NailingEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().getSpeed().addBase(-speed_reduction_);
}

void rpg::NailingEffect::doOnTick(rpg::Actor*)
{}

void rpg::NailingEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getSpeed().addBase(speed_reduction_);
}

rpg::BleedingEffect::BleedingEffect(size_t duration, float percentage_damage, float flat_damage):
  rpg::Effect(rpg::EffectType::Bleeding, duration),
  percentage_damage_per_move_(percentage_damage),
  flat_damage_per_move_(flat_damage)
{}

rpg::BleedingEffect::BleedingEffect():
  rpg::BleedingEffect(3, 0.03f, 10.0f)
{}

bool rpg::BleedingEffect::isStackable() const
{
  return true;
}

void rpg::BleedingEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::BleedingEffect::clone() const
{
  return std::make_unique< rpg::BleedingEffect >(*this);
}

void rpg::BleedingEffect::doOnApply(rpg::Actor*)
{}

void rpg::BleedingEffect::doOnTick(rpg::Actor* owner)
{
  owner->getDamageManager().takeDamage(flat_damage_per_move_);
  owner->getDamageManager().takeDamage(owner->getStats().getHealth().getTotal() * percentage_damage_per_move_);
}

void rpg::BleedingEffect::doOnRemove(rpg::Actor*)
{}

rpg::ParryEffect::ParryEffect(size_t duration, float parry_chance):
  rpg::Effect(rpg::EffectType::Parry, duration),
  parry_chance_(parry_chance)
{}

rpg::ParryEffect::ParryEffect():
  rpg::ParryEffect(3, 0.5f)
{}

bool rpg::ParryEffect::isStackable() const
{
  return false;
}

void rpg::ParryEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::ParryEffect::clone() const
{
  return std::make_unique< rpg::ParryEffect >(*this);
}

void rpg::ParryEffect::doOnApply(rpg::Actor*)
{}

void rpg::ParryEffect::doOnTick(rpg::Actor*)
{}

void rpg::ParryEffect::doOnRemove(rpg::Actor*)
{}

float rpg::ParryEffect::getParryChance() const
{
  return parry_chance_;
}

rpg::DamageBuffEffect::DamageBuffEffect(size_t duration, float buff):
  rpg::Effect(rpg::EffectType::DamageBuff, duration),
  damage_buff_(buff)
{}

rpg::DamageBuffEffect::DamageBuffEffect():
  rpg::DamageBuffEffect(3, 0.2f)
{}

bool rpg::DamageBuffEffect::isStackable() const
{
  return true;
}

void rpg::DamageBuffEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::DamageBuffEffect::clone() const
{
  return std::make_unique< rpg::DamageBuffEffect >(*this);
}

void rpg::DamageBuffEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().getDamage().addMultiply(damage_buff_);
}

void rpg::DamageBuffEffect::doOnTick(rpg::Actor*)
{}

void rpg::DamageBuffEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getDamage().addMultiply(-damage_buff_);
}

float rpg::DamageBuffEffect::getDamageBuff() const
{
  return damage_buff_;
}

rpg::BurnEffect::BurnEffect(size_t duration, float damage_per_tick):
  rpg::Effect(rpg::EffectType::Burn, duration),
  damage_per_tick_(damage_per_tick)
{}

rpg::BurnEffect::BurnEffect():
  rpg::BurnEffect(3, 15.0f)
{}

bool rpg::BurnEffect::isStackable() const
{
  return true;
}

void rpg::BurnEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::BurnEffect::clone() const
{
  return std::make_unique< rpg::BurnEffect >(*this);
}

void rpg::BurnEffect::doOnApply(rpg::Actor*)
{}

void rpg::BurnEffect::doOnTick(rpg::Actor* owner)
{
  owner->getDamageManager().takeDamage(damage_per_tick_);
}

void rpg::BurnEffect::doOnRemove(rpg::Actor*)
{}

rpg::SlowEffect::SlowEffect(size_t duration, float speed_multiplier):
  rpg::Effect(rpg::EffectType::Slow, duration),
  speed_multiplier_(speed_multiplier)
{}

rpg::SlowEffect::SlowEffect():
  rpg::SlowEffect(3, 0.5f)
{}

bool rpg::SlowEffect::isStackable() const
{
  return false;
}

void rpg::SlowEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::SlowEffect::clone() const
{
  return std::make_unique< rpg::SlowEffect >(*this);
}

void rpg::SlowEffect::doOnApply(rpg::Actor* owner)
{
  original_speed_ = owner->getStats().getSpeed().getTotal();
  owner->getStats().getSpeed().setBase(original_speed_ * speed_multiplier_);
}

void rpg::SlowEffect::doOnTick(rpg::Actor*)
{}

void rpg::SlowEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getSpeed().setBase(original_speed_);
}

rpg::StunEffect::StunEffect(size_t duration):
  rpg::Effect(rpg::EffectType::Stun, duration)
{}

rpg::StunEffect::StunEffect():
  rpg::StunEffect(1)
{}

bool rpg::StunEffect::isStackable() const
{
  return false;
}

void rpg::StunEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::StunEffect::clone() const
{
  return std::make_unique< rpg::StunEffect >(*this);
}

void rpg::StunEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().setStunned(true);
}

void rpg::StunEffect::doOnTick(rpg::Actor*)
{}

void rpg::StunEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().setStunned(false);
}

rpg::TauntEffect::TauntEffect(size_t duration):
  rpg::Effect(rpg::EffectType::Taunt, duration)
{}

rpg::TauntEffect::TauntEffect():
  rpg::TauntEffect(2)
{}

bool rpg::TauntEffect::isStackable() const
{
  return false;
}

void rpg::TauntEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::TauntEffect::clone() const
{
  return std::make_unique< rpg::TauntEffect >(*this);
}

void rpg::TauntEffect::doOnApply(rpg::Actor*)
{}

void rpg::TauntEffect::doOnTick(rpg::Actor*)
{}

void rpg::TauntEffect::doOnRemove(rpg::Actor*)
{}

rpg::DefenseBuffEffect::DefenseBuffEffect(size_t duration, float multiplier):
  rpg::Effect(rpg::EffectType::DefenseBuff, duration),
  multiplier_(multiplier)
{}

rpg::DefenseBuffEffect::DefenseBuffEffect():
  rpg::DefenseBuffEffect(3, 1.3f)
{}

bool rpg::DefenseBuffEffect::isStackable() const
{
  return true;
}

void rpg::DefenseBuffEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::DefenseBuffEffect::clone() const
{
  return std::make_unique< rpg::DefenseBuffEffect >(*this);
}

void rpg::DefenseBuffEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().getDefense().addMultiply(multiplier_ - 1.0f);
}

void rpg::DefenseBuffEffect::doOnTick(rpg::Actor*)
{}

void rpg::DefenseBuffEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getDefense().addMultiply(-(multiplier_ - 1.0f));
}

float rpg::DefenseBuffEffect::getMultiplier() const
{
  return multiplier_;
}

rpg::SpeedBuffEffect::SpeedBuffEffect(size_t duration, float multiplier):
  rpg::Effect(rpg::EffectType::SpeedBuff, duration),
  multiplier_(multiplier)
{}

rpg::SpeedBuffEffect::SpeedBuffEffect():
  rpg::SpeedBuffEffect(3, 1.3f)
{}

bool rpg::SpeedBuffEffect::isStackable() const
{
  return true;
}

void rpg::SpeedBuffEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::SpeedBuffEffect::clone() const
{
  return std::make_unique< rpg::SpeedBuffEffect >(*this);
}

void rpg::SpeedBuffEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().getSpeed().addMultiply(multiplier_ - 1.0f);
}

void rpg::SpeedBuffEffect::doOnTick(rpg::Actor*)
{}

void rpg::SpeedBuffEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getSpeed().addMultiply(-(multiplier_ - 1.0f));
}

float rpg::SpeedBuffEffect::getMultiplier() const
{
  return multiplier_;
}

rpg::InvincibilityEffect::InvincibilityEffect(size_t duration):
  rpg::Effect(rpg::EffectType::Invincibility, duration)
{}

rpg::InvincibilityEffect::InvincibilityEffect():
  rpg::InvincibilityEffect(2)
{}

bool rpg::InvincibilityEffect::isStackable() const
{
  return false;
}

void rpg::InvincibilityEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::InvincibilityEffect::clone() const
{
  return std::make_unique< rpg::InvincibilityEffect >(*this);
}

void rpg::InvincibilityEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().setInvincible(true);
}

void rpg::InvincibilityEffect::doOnTick(rpg::Actor*)
{}

void rpg::InvincibilityEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().setInvincible(false);
}

rpg::DamageAmplifyEffect::DamageAmplifyEffect(size_t duration, float multiplier):
  rpg::Effect(rpg::EffectType::DamageAmplify, duration),
  multiplier_(multiplier)
{}

rpg::DamageAmplifyEffect::DamageAmplifyEffect():
  rpg::DamageAmplifyEffect(3, 1.5f)
{}

bool rpg::DamageAmplifyEffect::isStackable() const
{
  return true;
}

void rpg::DamageAmplifyEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::DamageAmplifyEffect::clone() const
{
  return std::make_unique< rpg::DamageAmplifyEffect >(*this);
}

void rpg::DamageAmplifyEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().getDamageBonus().addBase(multiplier_);
}

void rpg::DamageAmplifyEffect::doOnTick(rpg::Actor*)
{}

void rpg::DamageAmplifyEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().getDamageBonus().addBase(-multiplier_);
}

float rpg::DamageAmplifyEffect::getMultiplier() const
{
  return multiplier_;
}

rpg::HealOverTimeEffect::HealOverTimeEffect(size_t duration, float percent_per_tick):
  rpg::Effect(rpg::EffectType::HealOverTime, duration),
  percent_per_tick_(percent_per_tick)
{}

rpg::HealOverTimeEffect::HealOverTimeEffect():
  rpg::HealOverTimeEffect(3, 0.1f)
{}

bool rpg::HealOverTimeEffect::isStackable() const
{
  return true;
}

void rpg::HealOverTimeEffect::doOnStack(rpg::Effect* effect)
{
  rpg::Effect::doOnStack(effect);
}

std::unique_ptr< rpg::Effect > rpg::HealOverTimeEffect::clone() const
{
  return std::make_unique< rpg::HealOverTimeEffect >(*this);
}

void rpg::HealOverTimeEffect::doOnApply(rpg::Actor*)
{}

void rpg::HealOverTimeEffect::doOnTick(rpg::Actor* owner)
{
  owner->getDamageManager().heal(owner->getStats().getHealth().getTotal() * percent_per_tick_);
}

void rpg::HealOverTimeEffect::doOnRemove(rpg::Actor*)
{}

rpg::ShieldEffect::ShieldEffect(size_t duration, float shield_amount):
  rpg::Effect(rpg::EffectType::Shield, duration),
  shield_amount_(shield_amount)
{}

rpg::ShieldEffect::ShieldEffect():
  rpg::ShieldEffect(3, 100.0f)
{}

bool rpg::ShieldEffect::isStackable() const
{
  return false;
}

void rpg::ShieldEffect::doOnStack(rpg::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< rpg::Effect > rpg::ShieldEffect::clone() const
{
  return std::make_unique< rpg::ShieldEffect >(*this);
}

void rpg::ShieldEffect::doOnApply(rpg::Actor* owner)
{
  owner->getStats().addShield(shield_amount_);
}

void rpg::ShieldEffect::doOnTick(rpg::Actor*)
{}

void rpg::ShieldEffect::doOnRemove(rpg::Actor* owner)
{
  owner->getStats().removeShield(shield_amount_);
}

bool rpg::NailingEffect::isHarmful() const
{
  return true;
}

bool rpg::BleedingEffect::isHarmful() const
{
  return true;
}

bool rpg::BurnEffect::isHarmful() const
{
  return true;
}

bool rpg::SlowEffect::isHarmful() const
{
  return true;
}

bool rpg::StunEffect::isHarmful() const
{
  return true;
}

bool rpg::TauntEffect::isHarmful() const
{
  return true;
}

bool rpg::DamageAmplifyEffect::isHarmful() const
{
  return true;
}

bool rpg::ParryEffect::isHarmful() const
{
  return false;
}

bool rpg::DamageBuffEffect::isHarmful() const
{
  return false;
}

bool rpg::DefenseBuffEffect::isHarmful() const
{
  return false;
}

bool rpg::SpeedBuffEffect::isHarmful() const
{
  return false;
}

bool rpg::ShieldEffect::isHarmful() const
{
  return false;
}

bool rpg::HealOverTimeEffect::isHarmful() const
{
  return false;
}

bool rpg::InvincibilityEffect::isHarmful() const
{
  return false;
}
