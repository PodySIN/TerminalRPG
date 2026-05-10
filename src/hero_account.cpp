#include "hero_account.hpp"
#include "hero_factory.hpp"

void rpg::UnitSaveData::applyTo(Hero& hero) const
{
  auto& stats = hero.getStats();

  stats.getHealth().setBase(base_health);
  stats.getHealth().addFlat(flat_health);
  stats.getHealth().setMultiply(health_multiply);
  stats.getHealth().setAmplify(health_amplify);

  stats.getDamage().setBase(base_damage);
  stats.getDamage().addFlat(flat_damage);
  stats.getDamage().setMultiply(damage_multiply);
  stats.getDamage().setAmplify(damage_amplify);

  stats.getDefense().setBase(base_defense);
  stats.getDefense().addFlat(flat_defense);
  stats.getDefense().setMultiply(defense_multiply);
  stats.getDefense().setAmplify(defense_amplify);

  stats.getSpeed().setBase(base_speed);
  stats.getSpeed().addFlat(flat_speed);
  stats.getSpeed().setMultiply(speed_multiply);
  stats.getSpeed().setAmplify(speed_amplify);

  stats.getEvasion().setBase(base_evasion);
  stats.getEvasion().addFlat(flat_evasion);
  stats.getEvasion().setMultiply(evasion_multiply);
  stats.getEvasion().setAmplify(evasion_amplify);

  stats.getResource().setBase(base_resource);
  stats.getResource().addFlat(flat_resource);
  stats.getResource().setMultiply(resource_multiply);
  stats.getResource().setAmplify(resource_amplify);

  stats.getCritChance().setBase(crit_chance);
  stats.getCritDamage().setBase(crit_damage);
  stats.getDamageBonus().setBase(damage_bonus);
  stats.getEffectResistance().setBase(effect_resistance);
  stats.getMagicResistance().setBase(magic_resistance);
  stats.getDamageReduction().setBase(damage_reduction);
  stats.getBlockDamage().setBase(block_damage);

  stats.getCurrentHealth() = current_health;
  stats.getCurrentResource() = current_resource;
}

void rpg::UnitSaveData::loadFrom(const Hero& hero)
{
  const auto& stats = hero.getStats();

  hero_class = hero.getHeroClass();
  current_health = stats.getCurrentHealth();
  current_resource = stats.getCurrentResource();

  base_health = stats.getHealth().getBase();
  flat_health = stats.getHealth().flat_;
  health_multiply = stats.getHealth().multiply_;
  health_amplify = stats.getHealth().amplify_;

  base_damage = stats.getDamage().getBase();
  flat_damage = stats.getDamage().flat_;
  damage_multiply = stats.getDamage().multiply_;
  damage_amplify = stats.getDamage().amplify_;

  base_defense = stats.getDefense().getBase();
  flat_defense = stats.getDefense().flat_;
  defense_multiply = stats.getDefense().multiply_;
  defense_amplify = stats.getDefense().amplify_;

  base_speed = stats.getSpeed().getBase();
  flat_speed = stats.getSpeed().flat_;
  speed_multiply = stats.getSpeed().multiply_;
  speed_amplify = stats.getSpeed().amplify_;

  base_evasion = stats.getEvasion().getBase();
  flat_evasion = stats.getEvasion().flat_;
  evasion_multiply = stats.getEvasion().multiply_;
  evasion_amplify = stats.getEvasion().amplify_;

  base_resource = stats.getResource().getBase();
  flat_resource = stats.getResource().flat_;
  resource_multiply = stats.getResource().multiply_;
  resource_amplify = stats.getResource().amplify_;

  crit_chance = stats.getCritChance().getBase();
  crit_damage = stats.getCritDamage().getBase();
  damage_bonus = stats.getDamageBonus().getBase();
  effect_resistance = stats.getEffectResistance().getBase();
  magic_resistance = stats.getMagicResistance().getBase();
  damage_reduction = stats.getDamageReduction().getBase();
  block_damage = stats.getBlockDamage().getBase();
}

void rpg::HeroAccount::applyToHeroes()
{
  if (main_hero_ptr) {
    main_hero.applyTo(*main_hero_ptr);
  }

  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].applyTo(*party_ptrs[i]);
    }
  }
}

void rpg::HeroAccount::loadFromHeroes()
{
  if (main_hero_ptr) {
    main_hero.loadFrom(*main_hero_ptr);
  }

  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].loadFrom(*party_ptrs[i]);
    }
  }
}

rpg::HeroAccount::HeroAccount(const std::string& hero_name,
                              HeroClass hero_class) :
  name(hero_name)
{
  main_hero_ptr = createHeroByClass(hero_class);
  main_hero.loadFrom(*main_hero_ptr);
}
