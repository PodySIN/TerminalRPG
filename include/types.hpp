#ifndef TYPES_HPP
#define TYPES_HPP

namespace rpg {
  enum class ActorType {
    Hero,
    Monster,
    Npc
  };
  enum class ScaleType {
    Damage,
    Health,
    Defense,
    Resource
  };

  enum class DamageType {
    Physical,
    Magic,
    Pure
  };

  enum class EffectType {
    Poisoned,
    Stunned,
    Blinded,
    Accurate,
    Nailing,
    Bleeding
  };
} // namespace rpg

#endif
