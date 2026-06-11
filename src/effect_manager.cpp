#include "effect_manager.hpp"
#include <memory>
#include "actor.hpp"
#include "effects.hpp"

rpg::EffectManager::EffectManager(Actor* owner):
  owner_(owner)
{}

void rpg::EffectManager::addEffect(std::unique_ptr< Effect > effect)
{
  if (!effect) {
    return;
  }
  if (effect->isStackable()) {
    effect->onApply(owner_);
    effects_.push_back(std::move(effect));
  } else {
    Effect* existing = getEffect(effect->getEffectType());
    if (!existing) {
      effect->onApply(owner_);
      effects_.push_back(std::move(effect));
    } else {
      existing->doOnStack(effect.get());
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

rpg::Effect* rpg::EffectManager::getEffect(EffectType type) const
{
  for (const auto& effect : effects_) {
    if (effect && effect->getEffectType() == type) {
      return effect.get();
    }
  }
  return nullptr;
}

bool rpg::EffectManager::hasEffect(EffectType type) const
{
  return getEffect(type) != nullptr;
}

void rpg::EffectManager::removeAllEffects()
{
  for (auto& effect : effects_) {
    if (effect) {
      effect->onRemove(owner_);
    }
  }
  effects_.clear();
}

void rpg::EffectManager::removeNegativeEffects()
{
  for (auto it = effects_.begin(); it != effects_.end();) {
    if ((*it)->isHarmful()) {
      (*it)->onRemove(owner_);
      it = effects_.erase(it);
    } else {
      ++it;
    }
  }
}
