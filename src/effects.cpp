#include "effects.hpp"
#include "actor.hpp"

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
