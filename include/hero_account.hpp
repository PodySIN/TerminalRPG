#ifndef HERO_ACCOUNT_HPP
#define HERO_ACCOUNT_HPP

#include "hero.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace rpg {

  struct UnitSaveData {
    std::string name = "";
    HeroClass hero_class;
    float current_health = 100.0f;
    float current_resource = 50.0f;

    float base_health = 100.0f;
    float flat_health = 0.0f;
    float health_multiply = 1.0f;
    float health_amplify = 0.0f;

    float base_damage = 20.0f;
    float flat_damage = 0.0f;
    float damage_multiply = 1.0f;
    float damage_amplify = 0.0f;

    float base_defense = 10.0f;
    float flat_defense = 0.0f;
    float defense_multiply = 1.0f;
    float defense_amplify = 0.0f;

    float base_speed = 100.0f;
    float flat_speed = 0.0f;
    float speed_multiply = 1.0f;
    float speed_amplify = 0.0f;

    float base_evasion = 0.05f;
    float flat_evasion = 0.0f;
    float evasion_multiply = 1.0f;
    float evasion_amplify = 0.0f;

    float base_resource = 100.0f;
    float flat_resource = 0.0f;
    float resource_multiply = 1.0f;
    float resource_amplify = 0.0f;

    float crit_chance = 0.05f;
    float crit_damage = 1.5f;
    float damage_bonus = 0.0f;
    float effect_resistance = 0.0f;
    float magic_resistance = 0.0f;
    float damage_reduction = 0.0f;
    float block_damage = 0.0f;

    void applyTo(Hero& hero) const;
    void loadFrom(const Hero& hero);
  };

  struct HeroAccount {
    std::string name;
    size_t current_stage = 1;
    UnitSaveData main_hero;
    std::unique_ptr< Hero > main_hero_ptr;
    HeroAccount() = default;
    HeroAccount(const std::string& hero_name, HeroClass hero_class);
    size_t party_size = 0;
    std::vector< UnitSaveData > party;
    std::vector< std::unique_ptr< Hero > > party_ptrs;

    void applyToHeroes();
    void loadFromHeroes();
  };

} // namespace rpg

#endif
