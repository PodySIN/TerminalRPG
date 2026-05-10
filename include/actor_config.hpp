#ifndef ACTOR_CONFIG_HPP
#define ACTOR_CONFIG_HPP
#include "types.hpp"

namespace rpg {

  struct ActorConfig {
    float health = 100.0f;
    float resource = 100.0f;
    float damage = 10.0f;
    float defense = 5.0f;
    float speed = 100.0f;
    float crit_chance = 0.05f;
    float crit_damage = 1.0f;
    float evasion = 100.0f;
    float damage_bonus = 0.0f;
    float damage_reduction = 0.0f;
    float effect_resistance = 0.0f;
    HeroClass hero_class = HeroClass::Knight;
    ActorType actor_type = ActorType::Hero;
    ActorConfig() = default;
  };

  namespace Presets {
    ActorConfig Knight();
    ActorConfig Mage();
    ActorConfig Goblin();
  } // namespace Presets
} // namespace rpg

#endif
