#ifndef SKILLS_HPP
#define SKILLS_HPP

#include "types.hpp"
#include <string>
#include <vector>

namespace rpg {
  class Actor;
  class Effect;

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

  class KnightSlash : public AttackSkill {
  public:
    KnightSlash();

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

} // namespace rpg

#endif
