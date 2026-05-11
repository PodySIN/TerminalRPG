#ifndef DAMAGE_MANAGER_HPP
#define DAMAGE_MANAGER_HPP

#include "skills.hpp"

namespace rpg {

  class DamageManager {
  public:
    DamageManager(Actor* owner);
    float calculateOutputDamage(AttackSkill* skill);
    bool handleAttack(float attack, Actor* attacker);
    float calculateInputDamage(float attack);
    void takeDamage(float damage);

  private:
    Actor* owner_;
  };

} // namespace rpg

#endif
