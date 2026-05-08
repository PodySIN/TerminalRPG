#include "actor_config.hpp"

namespace rpg {
  namespace Presets {
    ActorConfig Knight()
    {
      ActorConfig config;
      config.health = 200.0f;
      config.resource = 100.0f;
      config.damage = 25.0f;
      config.defense = 30.0f;
      config.speed = 80.0f;
      config.crit_chance = 0.05f;
      config.crit_damage = 1.0f;
      config.evasion = 5.0f;
      config.damage_bonus = 1.0f;
      config.damage_reduction = 0.0f;
      config.effect_resistance = 0.0f;
      config.actor_type = ActorType::Hero;
      return config;
    }

    ActorConfig Mage()
    {
      ActorConfig config;
      config.health = 200.0f;
      config.resource = 100.0f;
      config.damage = 25.0f;
      config.defense = 30.0f;
      config.speed = 80.0f;
      config.crit_chance = 0.05f;
      config.crit_damage = 1.0f;
      config.evasion = 0.0f;
      config.damage_bonus = 1.0f;
      config.damage_reduction = 0.0f;
      config.effect_resistance = 0.0f;
      config.actor_type = ActorType::Hero;
      return config;
    }

    ActorConfig Goblin()
    {
      ActorConfig config;
      config.health = 200.0f;
      config.resource = 100.0f;
      config.damage = 25.0f;
      config.defense = 30.0f;
      config.speed = 80.0f;
      config.crit_chance = 0.05f;
      config.crit_damage = 1.0f;
      config.evasion = 0.0f;
      config.damage_bonus = 1.0f;
      config.damage_reduction = 0.0f;
      config.effect_resistance = 0.0f;
      config.actor_type = ActorType::Hero;
      return config;
    }

  } // namespace Presets
} // namespace rpg
