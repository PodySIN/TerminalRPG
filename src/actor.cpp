#include "actor.hpp"
#include "damage_manager.hpp"
#include "effect_manager.hpp"
#include "skill_manager.hpp"
#include "types.hpp"

rpg::CoreStats& rpg::Actor::getStats()
{
  return stats_;
}

rpg::EffectManager& rpg::Actor::getEffectManager()
{
  return effect_manager_;
}

rpg::SkillManager& rpg::Actor::getSkillManager()
{
  return skill_manager_;
}

const rpg::CoreStats& rpg::Actor::getStats() const
{
  return stats_;
}

const rpg::EffectManager& rpg::Actor::getEffectManager() const
{
  return effect_manager_;
}

const rpg::SkillManager& rpg::Actor::getSkillManager() const
{
  return skill_manager_;
}

rpg::DamageManager& rpg::Actor::getDamageManager()
{
  return damage_manager_;
}

const rpg::DamageManager& rpg::Actor::getDamageManager() const
{
  return damage_manager_;
}

rpg::ActorType rpg::Actor::getActorType() const
{
  return actor_type_;
}

rpg::Actor::Actor(const ActorConfig& config) :
  stats_(),
  effect_manager_(this),
  skill_manager_(this),
  damage_manager_(this),
  actor_type_(config.actor_type)
{
  getStats().getHealth().setBase(config.health);
  getStats().getResource().setBase(config.resource);
  getStats().getDefense().setBase(config.defense);
  getStats().getSpeed().setBase(config.speed);
  getStats().getDamage().setBase(config.damage);
  getStats().getCritChance().setBase(config.crit_chance);
  getStats().getCritDamage().setBase(config.crit_damage);
  getStats().getDamageBonus().setBase(config.damage_bonus);
  getStats().getDamageReduction().setBase(config.damage_reduction);
  getStats().getCurrentHealth() = config.health;
}
