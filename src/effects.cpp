#include "effects.hpp"
#include "actor.hpp"
#include <memory>

rpg::Effect::Effect(EffectType type, size_t duration) :
  type_(type), duration_(duration)
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

void rpg::Effect::setDuration(float value)
{
  duration_ = value;
}

rpg::NailingEffect::NailingEffect(size_t duration, float speed_reduction) :
  Effect(EffectType::Nailing, duration), speed_reduction_(speed_reduction)
{
}

rpg::NailingEffect::NailingEffect() : NailingEffect(3, 0.2f)
{
}

bool rpg::NailingEffect::isStackable() const
{
  return true;
}

std::unique_ptr< rpg::Effect > rpg::NailingEffect::clone() const
{
  return std::make_unique< NailingEffect >(*this);
}

void rpg::NailingEffect::doOnApply(Actor* owner)
{
  auto& speed = owner->getStats().getSpeed();
  speed.addBase(-speed_reduction_);
}

void rpg::NailingEffect::doOnTick(Actor*)
{
  return;
}

void rpg::NailingEffect::doOnRemove(Actor* owner)
{
  auto& speed = owner->getStats().getSpeed();
  speed.addBase(speed_reduction_);
}

rpg::BleedingEffect::BleedingEffect(size_t duration,
                                    float percetage_damage_per_move,
                                    float flat_damage_per_move) :
  Effect(EffectType::Bleeding, duration),
  percetage_damage_per_move_(percetage_damage_per_move),
  flat_damage_per_move_(flat_damage_per_move)
{
}

rpg::BleedingEffect::BleedingEffect() : BleedingEffect(2, 0.03f, 1.0f)
{
}

bool rpg::BleedingEffect::isStackable() const
{
  return true;
}

std::unique_ptr< rpg::Effect > rpg::BleedingEffect::clone() const
{
  return std::make_unique< BleedingEffect >();
}

void rpg::BleedingEffect::doOnApply(Actor*)
{
  return;
}

void rpg::BleedingEffect::doOnTick(Actor* owner)
{
  owner->getDamageManager().takeDamage(
      owner->getStats().getHealth().getTotal() * percetage_damage_per_move_);
  owner->getDamageManager().takeDamage(flat_damage_per_move_);
}

void rpg::BleedingEffect::doOnRemove(Actor*)
{
  return;
}

rpg::ParryEffect::ParryEffect(size_t duration, float parry_chance) :
  Effect(EffectType::Parry, duration), parry_chance_(parry_chance)
{
}

rpg::ParryEffect::ParryEffect() : rpg::ParryEffect(3.0f, 0.5f)
{
}

bool rpg::ParryEffect::isStackable() const
{
  return false;
}

std::unique_ptr< rpg::Effect > rpg::ParryEffect::clone() const
{
  return std::make_unique< ParryEffect >();
}

void rpg::ParryEffect::doOnApply(Actor*)
{
  return;
}

void rpg::ParryEffect::doOnTick(Actor*)
{
  return;
}

void rpg::ParryEffect::doOnRemove(Actor*)
{
  return;
}

float rpg::ParryEffect::getParryChance() const
{
  return parry_chance_;
}

rpg::DamageBuffEffect::DamageBuffEffect(size_t duration, float buff) :
  Effect(EffectType::DamageBuff, duration), damage_buff_(buff)
{
}

rpg::DamageBuffEffect::DamageBuffEffect() : rpg::DamageBuffEffect(3.0f, 0.2f)
{
}

bool rpg::DamageBuffEffect::isStackable() const
{
  return true;
}

std::unique_ptr< rpg::Effect > rpg::DamageBuffEffect::clone() const
{
  return std::make_unique< DamageBuffEffect >();
}

void rpg::DamageBuffEffect::doOnApply(Actor* owner)
{
  owner->getStats().getDamage().addMultiply(getDamageBuff());
}

void rpg::DamageBuffEffect::doOnTick(Actor*)
{
  return;
}

void rpg::DamageBuffEffect::doOnRemove(Actor* owner)
{
  owner->getStats().getDamage().addMultiply(-getDamageBuff());
}

float rpg::DamageBuffEffect::getDamageBuff() const
{
  return damage_buff_;
}
void rpg::Effect::doOnStack(Effect*)
{
  return;
}

void rpg::ParryEffect::doOnStack(Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

void rpg::NailingEffect::doOnStack(Effect* effect)
{
  Effect::doOnStack(effect);
}

void rpg::BleedingEffect::doOnStack(Effect* effect)
{
  Effect::doOnStack(effect);
}

void rpg::DamageBuffEffect::doOnStack(Effect* effect)
{
  Effect::doOnStack(effect);
}
