#include "effects.hpp"
#include "actor.hpp"
#include <memory>

rpg::Effect::Effect(EffectType type, size_t duration, float apply_chance) :
  type_(type), duration_(duration), apply_chance_(apply_chance)
{
}

void rpg::Effect::onApply(Actor* owner)
{
  doOnApply(owner);
}

void rpg::Effect::onTick(Actor* owner)
{
  doOnTick(owner);
  reduceDuration();
}

void rpg::Effect::onRemove(Actor* owner)
{
  doOnRemove(owner);
}

bool rpg::Effect::isExpired() const
{
  return duration_ == 0;
}

void rpg::Effect::reduceDuration()
{
  if (duration_ > 0) {
    duration_--;
  }
}

rpg::EffectType rpg::Effect::getEffectType() const
{
  return type_;
}

float rpg::Effect::getDuration() const
{
  return duration_;
}

float rpg::Effect::getApplyChance() const
{
  return apply_chance_;
}

rpg::NailingEffect::NailingEffect(size_t duration, float apply_chance,
                                  float evasion_reduction) :
  Effect(EffectType::Nailing, duration, apply_chance), evasion_reduction_(0.2f)
{
}

rpg::NailingEffect::NailingEffect() : NailingEffect(3, 1.0f, 20.0f)
{
}

bool rpg::NailingEffect::isHarmful() const
{
  return true;
}

std::unique_ptr< rpg::Effect > rpg::NailingEffect::clone() const
{
  return std::make_unique< NailingEffect >(*this);
}

void rpg::NailingEffect::doOnApply(Actor* owner)
{
  auto& evasion = owner->getStats().getEvasion();
  evasion.addMultiply(-evasion_reduction_);
}

void rpg::NailingEffect::doOnTick(Actor* owner)
{
  return;
}

void rpg::NailingEffect::doOnRemove(Actor* owner)
{
  auto& evasion = owner->getStats().getEvasion();
  evasion.addMultiply(evasion_reduction_);
}

rpg::BleedingEffect::BleedingEffect(size_t duration, float apply_chance,
                                    float percetage_damage_per_move,
                                    float flat_damage_per_move) :
  Effect(EffectType::Bleeding, duration, apply_chance),
  percetage_damage_per_move_(percetage_damage_per_move),
  flat_damage_per_move_(flat_damage_per_move)
{
}

rpg::BleedingEffect::BleedingEffect() : BleedingEffect(2, 1.0f, 0.03f, 1.0f)
{
}

bool rpg::BleedingEffect::isHarmful() const
{
  return true;
}

std::unique_ptr< rpg::Effect > rpg::BleedingEffect::clone() const
{
  return std::make_unique< BleedingEffect >();
}

void rpg::BleedingEffect::doOnApply(Actor* owner)
{
  return;
}

void rpg::BleedingEffect::doOnTick(Actor* owner)
{
  owner->getDamageManager().takeDamage(
      owner->getStats().getHealth().getTotal() * percetage_damage_per_move_);
  owner->getDamageManager().takeDamage(flat_damage_per_move_);
}

void rpg::BleedingEffect::doOnRemove(Actor* owner)
{
  return;
}

rpg::ParryEffect::ParryEffect(size_t duration, float apply_chance,
                              float parry_chance,
                              float parry_damage_multiplier) :
  Effect(EffectType::Parry, duration, apply_chance),
  parry_chance_(parry_chance),
  parry_damage_multiplier_(parry_damage_multiplier)
{
}

rpg::ParryEffect::ParryEffect() : rpg::ParryEffect(3, 1.0f, 0.5f, 0.75f)
{
}

bool rpg::ParryEffect::isHarmful() const
{
  return false;
}

std::unique_ptr< rpg::Effect > rpg::ParryEffect::clone() const
{
  return std::make_unique< ParryEffect >();
}

void rpg::ParryEffect::doOnApply(Actor* owner)
{
  return;
}

void rpg::ParryEffect::doOnTick(Actor* owner)
{
  return;
}

void rpg::ParryEffect::doOnRemove(Actor* owner)
{
  return;
}

float rpg::ParryEffect::getParryChance() const
{
  return parry_chance_;
}

rpg::BlockDamageBuffEffect::BlockDamageBuffEffect(size_t duration,
                                                  float apply_chance,
                                                  float block_damage_buff) :
  Effect(EffectType::Parry, duration, apply_chance),
  block_damage_buff_(block_damage_buff)
{
}

rpg::BlockDamageBuffEffect::BlockDamageBuffEffect() :
  rpg::BlockDamageBuffEffect(3, 1.0f, 5)
{
}

bool rpg::BlockDamageBuffEffect::isHarmful() const
{
  return false;
}

std::unique_ptr< rpg::Effect > rpg::BlockDamageBuffEffect::clone() const
{
  return std::make_unique< BlockDamageBuffEffect >();
}

void rpg::BlockDamageBuffEffect::doOnApply(Actor* owner)
{
  owner->getStats().getBlockDamage().addBase(getBlockDamageBuff());
}

void rpg::BlockDamageBuffEffect::doOnTick(Actor* owner)
{
  return;
}

void rpg::BlockDamageBuffEffect::doOnRemove(Actor* owner)
{
  return;
}

float rpg::BlockDamageBuffEffect::getBlockDamageBuff() const
{
  return block_damage_buff_;
}
