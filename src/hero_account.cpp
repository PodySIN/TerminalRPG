#include "hero_account.hpp"

void rpg::UnitSaveData::applyTo(Hero& hero) const
{
  auto& stats = hero.getStats();

  stats.getHealth().setBase(base_health);
  stats.getHealth().addFlat(flat_health);
  stats.getHealth().setMultiply(health_multiply);

  stats.getDamage().setBase(base_damage);
  stats.getDamage().addFlat(flat_damage);
  stats.getDamage().setMultiply(damage_multiply);

  stats.getDefense().setBase(base_defense);
  stats.getDefense().addFlat(flat_defense);
  stats.getDefense().setMultiply(defense_multiply);

  stats.getSpeed().setBase(base_speed);
  stats.getSpeed().addFlat(flat_speed);
  stats.getSpeed().setMultiply(speed_multiply);

  stats.getResource().setBase(base_resource);
  stats.getResource().addFlat(flat_resource);
  stats.getResource().setMultiply(resource_multiply);

  stats.getCritChance().setBase(crit_chance);
  stats.getCritDamage().setBase(crit_damage);
  stats.getDamageBonus().setBase(damage_bonus);
  stats.getDamageReduction().setBase(damage_reduction);

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

  base_damage = stats.getDamage().getBase();
  flat_damage = stats.getDamage().flat_;
  damage_multiply = stats.getDamage().multiply_;

  base_defense = stats.getDefense().getBase();
  flat_defense = stats.getDefense().flat_;
  defense_multiply = stats.getDefense().multiply_;

  base_speed = stats.getSpeed().getBase();
  flat_speed = stats.getSpeed().flat_;
  speed_multiply = stats.getSpeed().multiply_;

  base_resource = stats.getResource().getBase();
  flat_resource = stats.getResource().flat_;
  resource_multiply = stats.getResource().multiply_;

  crit_chance = stats.getCritChance().getBase();
  crit_damage = stats.getCritDamage().getBase();
  damage_bonus = stats.getDamageBonus().getBase();
  damage_reduction = stats.getDamageReduction().getBase();
}

void rpg::HeroAccount::applyToHeroes()
{
  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].applyTo(*party_ptrs[i]);
    }
  }
}

void rpg::HeroAccount::loadFromHeroes()
{
  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].loadFrom(*party_ptrs[i]);
    }
  }
}

rpg::HeroAccount::HeroAccount(const std::string& name) : account_name(name)
{
}
