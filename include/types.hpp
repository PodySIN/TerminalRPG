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

  enum class HeroClass {
    NoClass,
    Knight,
    Mage,
    Assasin,
    Palladin,
    Saint,
    Priest,
    Archer,
    Samurai,
    Engineer,
    Hunter
  };

  enum class StatType {
    Health,
    Damage,
    Evasion,
    Defense,
    Speed,
    CritChance,
    CritDamage,
    MagicResistance,
    BlockDamage,
    Resource
  };

  enum class KnightRareReward {
  };

  enum class KnightEpicReward {
  };

  enum class EffectType {
    Poisoned,
    Stunned,
    Blinded,
    Accurate,
    Nailing,
    Bleeding,
    Parry,
    DamageBuff
  };

} // namespace rpg

#endif
