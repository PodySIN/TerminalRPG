#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "types.hpp"
#include <cstddef>

namespace rpg {
  class Actor;
  class Effect {
  public:
    virtual ~Effect() = default;
    void onApply(Actor* owner);
    void onTick(Actor* owner);
    void onRemove(Actor* owner);
    ActorType getActorType() const;
    bool isExpired() const;
    float getDuration() const;
    float getApplyChance() const;

  protected:
    void reduceDuration();

  private:
    EffectType type_;
    size_t duration_;
    float apply_chance_;
    ActorType actor_type_;

    virtual void doOnApply(Actor* owner) = 0;
    virtual void doOnTick(Actor* owner) = 0;
    virtual void doOnRemove(Actor* owner) = 0;
  };

  class NailingEffect : public Effect {
  private:
    float evasion_reduction_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };
} // namespace rpg

#endif
