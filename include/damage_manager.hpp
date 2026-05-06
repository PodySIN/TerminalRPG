#ifndef DAMAGE_MANAGER_HPP
#define DAMAGE_MANAGER_HPP

#include "skills.hpp"
#include "types.hpp"

namespace rpg {

  class DamageManager {
  public:
    DamageManager(Actor* owner);
    std::pair< float, DamageType > calculateOutputDamage(AttackSkill* skill);
    void handleAttack(std::pair< float, DamageType >& attack);
    float calculateInputDamage(std::pair< float, DamageType >& attack);

    float calculateDodgeChance();
    void takeDamage(float damage);

  private:
    Actor* owner_;
  };

} // namespace rpg

#endif
