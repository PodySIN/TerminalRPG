#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "types.hpp"
#include <cstddef>
#include <memory>

namespace rpg {
  class Actor;
  class Effect {
  public:
    Effect(EffectType type, size_t duration, float apply_chance_);
    virtual ~Effect() = default;
    void onApply(Actor* owner);
    void onTick(Actor* owner);
    void onRemove(Actor* owner);
    bool isExpired() const;
    virtual bool isHarmful() const = 0;
    float getDuration() const;
    float getApplyChance() const;
    virtual std::unique_ptr< Effect > clone() const = 0;
    EffectType getEffectType() const;

  protected:
    void reduceDuration();

  private:
    EffectType type_;
    size_t duration_ = 3;
    float apply_chance_ = 1.0f;

    virtual void doOnApply(Actor* owner) = 0;
    virtual void doOnTick(Actor* owner) = 0;
    virtual void doOnRemove(Actor* owner) = 0;
  };

  class NailingEffect : public Effect {
  public:
    NailingEffect(size_t duration, float apply_chance, float evasion_reduction);
    NailingEffect();
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float evasion_reduction_ = 20.0f;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class BleedingEffect : public Effect {
  public:
    BleedingEffect(size_t duration, float apply_chance,
                   float percetage_damage_per_move, float flat_damage_per_move);
    BleedingEffect();
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float percetage_damage_per_move_ = 2.0f;
    float flat_damage_per_move_ = 10.0f;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class ParryEffect : public Effect {
  public:
    ParryEffect(size_t duration, float apply_chance, float parry_chance);
    ParryEffect();
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getParryChance() const;

  private:
    float parry_chance_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class BlockDamageBuffEffect : public Effect {
  public:
    BlockDamageBuffEffect(size_t duration, float apply_chance, float buff);
    BlockDamageBuffEffect();
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getBlockDamageBuff() const;

  private:
    float block_damage_buff_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };
} // namespace rpg

#endif
