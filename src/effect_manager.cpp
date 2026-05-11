#include "effect_manager.hpp"
#include "actor.hpp"
#include "effects.hpp"
#include <memory>

rpg::EffectManager::EffectManager(Actor* owner) : owner_(owner)
{
}

void rpg::EffectManager::addEffect(std::unique_ptr< Effect > effect)
{
  if (effect->isStackable()) {
    effect->onApply(owner_);
    effects_.push_back(std::move(effect));
  } else {
    std::unique_ptr< Effect > effect_on_owner =
        owner_->getEffectManager().isActorHasEffect(effect->getEffectType());
    if (!effect_on_owner) {
      effect->onApply(owner_);
      effects_.push_back(std::move(effect));
    } else {
      effect_on_owner->doOnStack(effect.get());
    }
  }
}

void rpg::EffectManager::update()
{
  for (size_t i = 0; i < effects_.size();) {
    effects_[i]->onTick(owner_);
    if (effects_[i]->isExpired()) {
      effects_[i]->onRemove(owner_);
      if (i != effects_.size() - 1) {
        effects_[i] = std::move(effects_.back());
      }
      effects_.pop_back();
    } else {
      i++;
    }
  }
}

std::unique_ptr< rpg::Effect >
rpg::EffectManager::isActorHasEffect(EffectType type) const
{
  for (const auto& effect : effects_) {
    if (effect->getEffectType() == type) {
      return std::unique_ptr< Effect >(effect.get());
    }
  }
  return nullptr;
}
