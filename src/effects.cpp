#include "effects.hpp"
#include "actor.hpp"

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

rpg::ActorType rpg::Effect::getActorType() const
{
  return actor_type_;
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
