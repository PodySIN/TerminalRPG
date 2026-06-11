#ifndef BASEACTOR_HPP
#define BASEACTOR_HPP

#include "actor_config.hpp"
#include "core_stats.hpp"
#include "damage_manager.hpp"
#include "effect_manager.hpp"
#include "skill_manager.hpp"
#include "types.hpp"

namespace rpg {
  HeroClass getHeroClassByString(std::string class_name);

  class Actor {
  public:
    Actor(const ActorConfig& config);
    virtual ~Actor() = default;
    CoreStats& getStats();
    const CoreStats& getStats() const;
    EffectManager& getEffectManager();
    const EffectManager& getEffectManager() const;
    SkillManager& getSkillManager();
    const SkillManager& getSkillManager() const;
    DamageManager& getDamageManager();
    const DamageManager& getDamageManager() const;
    ActorType getActorType() const;

    virtual void die() = 0;

  private:
    CoreStats stats_;
    EffectManager effect_manager_;
    SkillManager skill_manager_;
    DamageManager damage_manager_;
    ActorType actor_type_;
  };
}

#endif
