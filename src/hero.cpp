#include "hero.hpp"
#include "actor_config.hpp"
#include "types.hpp"

rpg::Hero::Hero(const ActorConfig& config) : Actor(config)
{
  hero_class_ = config.hero_class;
}

bool rpg::Hero::getIsDead() const
{
  return is_dead_;
}

void rpg::Hero::setIsDead(bool is_dead)
{
  is_dead_ = is_dead;
}

void rpg::Hero::die()
{
  setIsDead(true);
  return;
}

rpg::HeroClass rpg::Hero::getHeroClass() const
{
  return hero_class_;
}
