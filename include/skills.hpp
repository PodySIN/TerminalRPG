#ifndef SKILLS_HPP
#define SKILLS_HPP

#include "effects.hpp"
#include "types.hpp"
#include <string>
#include <vector>

namespace rpg {
  class Actor;

  class Skill {
  public:
    void processSkill(Actor* owner, Actor* target);
    void processSkill(Actor* owner, std::vector< Actor* > targets);
    virtual ~Skill() = default;
    std::string getSkillName() const;

  protected:
    std::string skill_name_;

  private:
    virtual void doProcessSkill(Actor* owner, Actor* target) = 0;
  };

  class AttackSkill : public Skill {
  public:
    float getDamageMultiplier() const;
    float getFlatDamage() const;
    DamageType getDamageType() const;
    ScaleType getScaleType() const;

  protected:
    float damage_multiplier_;
    float flat_damage_;
    DamageType damage_type_;
    ScaleType scale_type_;
  };

  class BaseAttack : public AttackSkill {
  public:
    BaseAttack(float damage_multiplier, float flat_damage,
               DamageType damage_type, ScaleType scale_type);
    BaseAttack();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ParryAttack : public AttackSkill {
  public:
    ParryAttack();
    ParryAttack(float damage_multiplier, ScaleType scale_type,
                DamageType damage_type);

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SavageSlash : public AttackSkill {
  public:
    SavageSlash();

  private:
    std::unique_ptr< Effect > effect_;
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class GuardianShield : public Skill {
    GuardianShield();

  private:
    std::unique_ptr< Effect > parry_effect_;
    std::unique_ptr< Effect > block_damage_buff_effect_;
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

} // namespace rpg

#endif
