#include "hero.hpp"
#include "actor_config.hpp"
#include "skills.hpp"

rpg::Hero::Hero(const ActorConfig& config) : Actor(config)
{
  auto slash = std::make_unique< KnightSlash >();
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
