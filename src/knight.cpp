#include "knight.hpp"
#include "actor_config.hpp"

rpg::Knight::Knight() : Hero(Presets::Knight())
{
}

void rpg::Knight::die()
{
  return;
}
