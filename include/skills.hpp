#ifndef SKILLS_HPP
#define SKILLS_HPP

#include <memory>
#include <string>
#include <vector>
#include "effects.hpp"
#include "types.hpp"

namespace rpg {

  class Actor;

  class Skill {
  public:
    void processSkill(Actor* owner, Actor* target);
    void processSkill(Actor* owner, std::vector< Actor* > targets);
    virtual ~Skill() = default;

    std::string getSkillName() const;
    std::string getDescription() const;
    void addSkillLevel(size_t value);
    size_t getSkillLevel() const;

  protected:
    std::string skill_name_;
    std::string description_;
    size_t skill_level_ = 1;

    virtual void doProcessSkill(Actor* owner, Actor* target) = 0;
  };

  class AttackSkill : public Skill {
  public:
    float getDamageMultiplier() const;
    float getFlatDamage() const;
    ScaleType getScaleType() const;
    float getLevelDamageScale() const;

  protected:
    float damage_multiplier_ = 1.0f;
    float flat_damage_ = 0.0f;
    ScaleType scale_type_ = ScaleType::Damage;
    float level_damage_scale_ = 0.05f;
  };

  class BaseAttack : public AttackSkill {
  public:
    BaseAttack();
    BaseAttack(float damage_multiplier, float flat_damage, ScaleType scale_type);

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightSlash : public AttackSkill {
  public:
    KnightSlash();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShieldBash : public AttackSkill {
  public:
    ShieldBash();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Taunt : public Skill {
  public:
    Taunt();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightProtection : public Skill {
  public:
    KnightProtection();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightUltimate1 : public AttackSkill {
  public:
    KnightUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightUltimate2 : public Skill {
  public:
    KnightUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Fireball : public AttackSkill {
  public:
    Fireball();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class IceShard : public AttackSkill {
  public:
    IceShard();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class LightningStrike : public AttackSkill {
  public:
    LightningStrike();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ManaShield : public Skill {
  public:
    ManaShield();
    ManaShield(float shield_amount, size_t duration);

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
    float shield_amount_;
    size_t duration_;
  };

  class MageUltimate1 : public AttackSkill {
  public:
    MageUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class MageUltimate2 : public Skill {
  public:
    MageUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Backstab : public AttackSkill {
  public:
    Backstab();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PoisonDagger : public AttackSkill {
  public:
    PoisonDagger();

  private:
    std::unique_ptr< Effect > effect_;
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShadowStep : public Skill {
  public:
    ShadowStep();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SpeedBoost : public Skill {
  public:
    SpeedBoost();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AssassinUltimate1 : public AttackSkill {
  public:
    AssassinUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AssassinUltimate2 : public AttackSkill {
  public:
    AssassinUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolyStrike : public AttackSkill {
  public:
    HolyStrike();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class DivineShield : public Skill {
  public:
    DivineShield();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolyLight : public Skill {
  public:
    HolyLight();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AuraOfProtection : public Skill {
  public:
    AuraOfProtection();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PaladinUltimate1 : public AttackSkill {
  public:
    PaladinUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PaladinUltimate2 : public AttackSkill {
  public:
    PaladinUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolySmite : public AttackSkill {
  public:
    HolySmite();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Heal : public Skill {
  public:
    Heal();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Purify : public Skill {
  public:
    Purify();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SaintUltimate1 : public Skill {
  public:
    SaintUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SaintUltimate2 : public Skill {
  public:
    SaintUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Smite : public AttackSkill {
  public:
    Smite();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HealPrayer : public Skill {
  public:
    HealPrayer();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShieldOfFaith : public Skill {
  public:
    ShieldOfFaith();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Renew : public Skill {
  public:
    Renew();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PriestUltimate1 : public Skill {
  public:
    PriestUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PriestUltimate2 : public Skill {
  public:
    PriestUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class QuickShot : public AttackSkill {
  public:
    QuickShot();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AimedShot : public AttackSkill {
  public:
    AimedShot();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PiercingArrow : public AttackSkill {
  public:
    PiercingArrow();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HawkEye : public Skill {
  public:
    HawkEye();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ArcherUltimate1 : public AttackSkill {
  public:
    ArcherUltimate1();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ArcherUltimate2 : public AttackSkill {
  public:
    ArcherUltimate2();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class DemonMark : public Skill {
  public:
    DemonMark();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Curse : public Skill {
  public:
    Curse();
    Curse(float damage_multiplier, int duration);

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
    float damage_multiplier_;
    int duration_;
  };
}

#endif
